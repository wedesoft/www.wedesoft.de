---
layout: post
title: Installing Pytorch with AMD ROCm on GNU/Linux
category: graphics
---

Quickly sharing my notes on how to install drivers for ROCm and Pytorch for machine learning on AMD GPUs:

First [install ROCm and the AMD GPU driver][1]:

{% highlight shell %}
# Install ROCm
wget https://repo.radeon.com/amdgpu-install/7.2.1/ubuntu/noble/amdgpu-install_7.2.1.70201-1_all.deb
sudo apt install ./amdgpu-install_7.2.1.70201-1_all.deb
# Install AMD driver
wget https://repo.radeon.com/amdgpu-install/7.2.1/ubuntu/noble/amdgpu-install_7.2.1.70201-1_all.deb
sudo apt install ./amdgpu-install_7.2.1.70201-1_all.deb
sudo apt update
sudo apt install "linux-headers-$(uname -r)"
sudo apt install amdgpu-dkms
{% endhighlight %}

Then as shown in this [Reddit post][2], I installed install triton, torch, torchvision, and torchaudio from [https://repo.radeon.com/rocm/manylinux/][3].

Then I tried the following program to train a neural network to imitate an XOR gate.

{% highlight python %}
import torch
import torch.nn as nn
import torch.optim as optim

# Check if GPU is available
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# XOR data
X = torch.tensor([[0, 0], [0, 1], [1, 0], [1, 1]], dtype=torch.float32).to(device)
Y = torch.tensor([[0], [1], [1], [0]], dtype=torch.float32).to(device)

# Define the neural network
class XORNet(nn.Module):
    def __init__(self):
        super(XORNet, self).__init__()
        self.fc1 = nn.Linear(2, 5)
        self.fc2 = nn.Linear(5, 1)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = self.sigmoid(self.fc2(x))
        return x

# Initialize the network, loss function and optimizer
model = XORNet().to(device)
criterion = nn.BCELoss()
optimizer = optim.SGD(model.parameters(), lr=0.1)

# Training loop
for epoch in range(10000):
    model.train()
    optimizer.zero_grad()
    outputs = model(X)
    loss = criterion(outputs, Y)
    loss.backward()
    optimizer.step()

    if (epoch+1) % 1000 == 0:
        print(f'Epoch [{epoch+1}/10000], Loss: {loss.item():.4f}')

# Test the model
model.eval()
with torch.no_grad():
    predictions = model(X)
    print("Predictions:", predictions.round())
{% endhighlight %}

However I got the following error (using Torch 2.9.1 and ROCm 7.2.0).

{% highlight shell %}
RuntimeError: CUDA error: HIPBLAS_STATUS_INVALID_VALUE when calling `hipblasLtMatmulAlgoGetHeuristic( ltHandle, computeDesc.descriptor(), Adesc.descriptor(), Bdesc.descriptor(), Cdesc.descriptor(), Cdesc.descriptor(), preference.descriptor(), 1, &heuristicResult, &returnedResult)`
{% endhighlight %}

Then I found AMD's information on [how to install Pytorch with ROCm support][4].
Basically you need to install the nightly build:

{% highlight shell %}
pip3 install --pre torch torchvision torchaudio --index-url https://download.pytorch.org/whl/nightly/rocm7.2
{% endhighlight %}

Now the XOR test works!

{% highlight shell %}
python3 xor.py
# Epoch [1000/10000], Loss: 0.0342
# Epoch [2000/10000], Loss: 0.0114
# Epoch [3000/10000], Loss: 0.0066
# Epoch [4000/10000], Loss: 0.0046
# Epoch [5000/10000], Loss: 0.0035
# Epoch [6000/10000], Loss: 0.0028
# Epoch [7000/10000], Loss: 0.0024
# Epoch [8000/10000], Loss: 0.0020
# Epoch [9000/10000], Loss: 0.0018
# Epoch [10000/10000], Loss: 0.0016
# Predictions: tensor([[0.],
#         [1.],
#         [1.],
#         [0.]], device='cuda:0')
{% endhighlight %}

Enjoy!

[1]: https://rocm.docs.amd.com/projects/install-on-linux/en/latest/install/quick-start.html
[2]: https://www.reddit.com/r/comfyui/comments/1njjp79/complete_rocm_70_pytorch_280_installation_guide/
[3]: https://repo.radeon.com/rocm/manylinux/
[4]: https://rocm.docs.amd.com/projects/install-on-linux/en/latest/install/3rd-party/pytorch-install.html#use-a-wheels-package
