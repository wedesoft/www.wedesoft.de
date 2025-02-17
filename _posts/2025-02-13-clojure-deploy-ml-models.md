---
layout: post
title: Deploying ML models in Clojure
category: software
image: /pics/onnx.png
---

Kira Howe 's 2024 article about [the current state of ML in Clojure][1] prominently features the [Tribuo][2] library by Oracle Labs and the [Clojure wrapper for Tribuo][3].
Tribuo integrates [XGBoost][8], [ONNX runtime][6], and [Tensorflow-Java][4].
However the Tensorflow bindings for Java look a bit verbose (see e.g. [MNIST example][5]).

Another approach is to train the model in Python, export it to the [ONNX][7] format and then use the [ONNX runtime][6] directly to perform inference in Clojure.
There is a recent [tutorial on using ONNX models from Clojure][9].
However it only deals with tabular data.

## Training

The following example uses [PyTorch][11] to train a traditional CNN classifier on the well-known [MNIST dataset][10] (the dataset can be obtained [here][16]).
The implementation performs the following steps:

* A class for reading MNIST images and labels is implemented.
* A CNN model using two convolutional layers and two fully connected layers is implemented and dropout regularization is applied.
* The training and test data is loaded as batches.
* The cross entropy loss function and an Adam optimizer are instantiated. Note that learning rate and dropout are hyperparameters which need to be tuned.
* The training loop performs prediction, loss computation, backpropagation, and optimization step.
* The test loop accumulates and displays the prediction accuracy on the test set.
* After 25 epochs, the models is exported to the ONNX format.

{% highlight python %}
import numpy as np
import torch
from torch import nn
from torch import onnx
from torch.nn import functional as F
from torch.utils.data import DataLoader, Dataset


class MNISTData(Dataset):

    def __init__(self, images_file_name, labels_file_name):
        """Read MNIST images and labels from specified files"""
        super(MNISTData, self).__init__()
        # Read images (skip magic, length, height, and width integers)
        self.images = np.fromfile(images_file_name, dtype=np.uint8)[16:].reshape(-1, 28, 28)
        # Read labels (skip magic and length integer)
        self.labels = np.fromfile(labels_file_name, dtype=np.uint8)[8:]

    def __len__(self):
        """Return the number of images (or labels) in the dataset"""
        return len(self.labels)

    def __getitem__(self, idx):
        """Return the image and label at the specified index"""
        image = torch.from_numpy(self.images[idx]).to(torch.float) / 255.0
        label = torch.zeros(10)
        label[self.labels[idx]] = 1
        return image, label


class MNISTNet(nn.Module):

    def __init__(self):
        """Construct network with 2 convolutional layers and 2 fully connected layers"""
        super(MNISTNet, self).__init__()
        self.conv1 = nn.Conv2d(1, 10, kernel_size=5)
        self.conv2 = nn.Conv2d(10, 20, kernel_size=5)
        self.conv2_drop = nn.Dropout2d(p=0.2)
        self.fc1 = nn.Linear(320, 50)
        self.fc2 = nn.Linear(50, 10)

    def forward(self, x):
        """Perform forward pass of network"""
        x = x.view(-1, 1, 28, 28)
        x = F.relu(F.max_pool2d(self.conv1(x), 2))
        x = F.relu(F.max_pool2d(self.conv2_drop(self.conv2(x)), 2))
        x = x.view(-1, 320)
        x = F.relu(self.fc1(x))
        x = F.dropout(x, p=0.2, training=self.training)
        x = self.fc2(x)
        return F.softmax(x, dim=1)


def main():
    train_data = MNISTData('data/train-images-idx3-ubyte', 'data/train-labels-idx1-ubyte')
    test_data = MNISTData('data/t10k-images-idx3-ubyte', 'data/t10k-labels-idx1-ubyte')

    train_loader = DataLoader(train_data, batch_size=64)
    test_loader = DataLoader(test_data, batch_size=64)

    model = MNISTNet()
    loss = nn.CrossEntropyLoss()
    # Adam optimizer
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

    for epoch in range(25):
        for x, y in train_loader:
            pred = model(x)
            l = loss(pred, y)
            optimizer.zero_grad()
            l.backward()
            optimizer.step()

        correct = 0
        total = 0
        for x, y in test_loader:
            pred = model(x).argmax(dim=1)
            correct += (pred == y.argmax(dim=1)).sum().item()
            total += len(y)
        print('Accuracy: {}'.format(correct / total))

    # Save model as ONNX
    torch.onnx.export(model,
                      (torch.randn((1, 1, 28, 28), dtype=torch.float),),
                      'mnist.onnx',
                      input_names=['input'],
                      output_names=['output'])
{% endhighlight %}

## Inference

The model file `mnist.onnx` can now be used for inference in Clojure.
The `deps.edn` file specifies the [ONNX runtime][6] and the [cljfx][12] library:

{% highlight clojure %}
{:deps {com.microsoft.onnxruntime/onnxruntime {:mvn/version "1.20.0"}
        cljfx/cljfx {:mvn/version "1.9.3"}}
 :paths ["."]
 :aliases {:infer {:main-opts ["-m" "infer.core"]}}}
{% endhighlight %}

The `infer.clj` file contains the code to run the inference on the model.
The code contains the following functions for inference:

* `read-digit` - Read a 28*28 gray-scale byte block from the MNIST dataset
* `feature-scaling` - Scale byte features to [0, 1] floating-point range. Note that Clojure byte arrays contain signed values which need to be converted to unsigned values!
* `argmax` - Return the index of the maximum value of a one-dimensional probability vector.
* `infer` - Convert a byte array to a ONNX tensor with batch size and number of channels being 1, run inference, and return the argmax of the probability vector.

Furthermore the `digit->image` function uses the idea shown in [James Thompson's Gist][13] to convert a byte array to a JavaFX image in order to display it.
The remaining code displays a small JavaFX GUI showing random images from the MNIST test data and the inference result.

{% highlight clojure %}
(ns infer.core
    (:require [clojure.java.io :as io]
              [cljfx.api :as fx])
    (:import [java.io ByteArrayOutputStream ByteArrayInputStream]
             [java.nio FloatBuffer]
             [javafx.application Platform]
             [ai.onnxruntime OrtEnvironment OrtSession OnnxTensor]))

(def environment (OrtEnvironment/getEnvironment))

(def mnist (-> environment (.createSession "mnist.onnx")))

(defn read-digit [n]
  "Read a 28*28 gray-scale byte block from the MNIST dataset."
  (with-open [in (io/input-stream "data/t10k-images-idx3-ubyte")]
    (.skip in (+ 16 (* n 28 28)))
    (.readNBytes in (* 28 28))))

(defn byte->ubyte [b]
  "Convert byte to unsigned byte"
  (if (>= b 0) b (+ b 256)))

(defn feature-scaling [digit]
  "Scale features to [0, 1] range"
  (float-array (map #(/ (byte->ubyte %) 255.0) digit)))

(defn argmax [arr]
  "Return the index of the maximum value in the array"
  (first
    (reduce (fn [[result maximum] [index value]] (if (> value maximum) [index value] [result maximum]))
            [0 (first arr)]
            (map vector (range) arr))))

(defn inference [digit]
  "Run inference on a digit image"
  (let [scaled        (feature-scaling digit)
        input-buffer  (FloatBuffer/wrap scaled)
        inputs        {"input" (OnnxTensor/createTensor environment input-buffer (long-array [1 1 28 28]))}
        outputs       (.run mnist inputs)
        output-tensor (.get (.get outputs "output"))
        output-buffer (.getFloatBuffer output-tensor)
        result        (float-array 10)]
    (.get output-buffer result)
    (argmax result)))

(defn digit->image [data]
  "Convert a 28*28 byte array to JavaFX image"
  (let [image  (java.awt.image.BufferedImage. 28 28 java.awt.image.BufferedImage/TYPE_BYTE_GRAY)
        raster (.getRaster image)
        out    (ByteArrayOutputStream.)]
    (.setDataElements raster 0 0 28 28 data)
    (javax.imageio.ImageIO/write image "png" out)
    (.flush out)
    (javafx.scene.image.Image. (ByteArrayInputStream. (.toByteArray out)))))

(def app-state (atom {:index (rand-int 10000)}))

(defn event-handler [& args]
  "Update application state with random index"
  (swap! app-state update :index (fn [_] (rand-int 10000))))

(defn display-image [{:keys [image]}]
  "Image display for cljfx GUI"
  {:fx/type :image-view
   :fit-width 256
   :fit-height 256
   :image image})

(defn next-button [_]
  "Next button for cljfx GUI"
  {:fx/type :button
   :text "Next"
   :on-action event-handler})

(defn root [{:keys [index]}]
  "Main window for cljfx GUI"
  (let [digit  (read-digit index)
        result (inference digit)]
    {:fx/type :stage
     :showing true
     :title "MNIST"
     :scene {:fx/type :scene
             :root {:fx/type :v-box
                    :padding 3
                    :spacing 5
                    :children [{:fx/type display-image :image (digit->image digit)}
                               {:fx/type :h-box
                                :padding 3
                                :spacing 5
                                :children [{:fx/type next-button}
                                           {:fx/type :label :text (str "result = " result)}]}]}}}))

(def renderer
  "Renderer for cljfx GUI"
  (fx/create-renderer
   :middleware (fx/wrap-map-desc assoc :fx/type root)))

(defn -main [& args]
  (Platform/setImplicitExit true)
  (fx/mount-renderer app-state renderer))
{% endhighlight %}

Here is a screenshot of the inference GUI:

![inference GUI screenshot](/pics/infer.png)

## GPU usage

For the MNIST example a CPU is sufficient for training and inference.
For larger models one needs to use a GPU.

In PyTorch one can use the [.to][14] method to move models and tensors to the GPU.
For inference in Clojure, one needs to install onnxruntime_gpu instead of onnxruntime.
Furthermore one needs to select a GPU device when creating a session:

{% highlight clojure %}
; ...
(def device-id 0)
(def options (OrtSession$SessionOptions.))
(.addCUDA options device-id)
(def environment (OrtEnvironment/getEnvironment))

(def mnist (-> environment (.createSession "mnist.onnx" options)))
; ...
{% endhighlight %}

## Conclusion

The [ONNX runtime][6] allows you to train models using PyTorch and deploy them in Clojure applications.
Furthermore there are [Tensorflow-Java bindings][4] however they are more verbose.
Hopefully the [Clojure Tribuo][3] bindings eventually will provide a more concise API for implementing ML models and training them.

When using byte arrays in Clojure to represent images, one needs to convert them to unsigned byte in order to obtain correct results.
In the example we also used feature scaling for faster convergence during training.

Also see [github.com/wedesoft/clojure-onnx][15] for source code.

Enjoy!

[1]: https://codewithkira.com/2024-04-04-state-of-clojure-ml.html
[2]: https://tribuo.org/
[3]: https://github.com/scicloj/scicloj.ml.tribuo
[4]: https://github.com/tensorflow/java
[5]: https://github.com/tensorflow/java-models/blob/master/src/main/java/org/tensorflow/model/examples/cnn/lenet/CnnMnist.java
[6]: https://onnxruntime.ai/
[7]: https://onnx.ai/
[8]: https://xgboost.ai/
[9]: https://scicloj.github.io/clojure-data-tutorials/projects/ml/onnx/onnx.html
[10]: https://en.wikipedia.org/wiki/MNIST_database
[11]: https://pytorch.org/
[12]: https://github.com/cljfx/cljfx
[13]: https://gist.github.com/jamesthompson/3344090
[14]: https://stackoverflow.com/questions/50954479/using-cuda-with-pytorch
[15]: https://github.com/wedesoft/clojure-onnx
[16]: https://github.com/fgnt/mnist
