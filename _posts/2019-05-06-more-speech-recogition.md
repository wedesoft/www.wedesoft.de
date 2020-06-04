---
layout: post
title: More minimal speech recognition using Tensorflow
category: ai
---

A [GRU][1] and a fully connected layer with softmax output can be used to recognise words in an audio stream (sequence to sequence).
I recorded a random sequence of 100 utterances of 4 words ("left", "right", "stop", "go") with a sampling rate of 11025Hz.
Furthermore 300 seconds of background noise were recorded.
All audio data was grouped into chunks of 512 samples.
The logarithm of the Fourier spectrum of each chunk was used as input for the GRU.
The initial state of the GRU (128 units) was set to zero.
The network was trained in a sequence-to-sequence setting to recognise words given a sequence of audio chunks.

![Speech recognition](/pics/speech2.svg)

The example was trained using gradient descent with learning rate alpha = 0.05.
The background noise was cycled randomly and words were inserted with random length pause between them.
The network was trained to output the word label 10 times after recognising a word.
The training took about one hour on a CPU.
The resulting example is more minimalistic than the [Tensorflow audio recognition example][2].
The algorithm is very similar to an exercise in Andrew Ng's [Deep learning specialization][3].

{% youtube "https://www.youtube.com/watch?v=ohZ1XMLDYuk" %}

**Update:**

See [CourseDuck for a curated list of machine learning online courses][4].

[1]: https://en.wikipedia.org/wiki/Gated_recurrent_unit
[2]: https://www.tensorflow.org/tutorials/sequences/audio_recognition
[3]: https://www.deeplearning.ai/
[4]: https://courseduck.com/programming/machine-learning/
