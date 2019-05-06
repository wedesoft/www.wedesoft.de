---
layout: post
title: Minimal speech recognition using Tensorflow
category: ai
---

An [LSTM][1] can be used to recognise words from audio data (sequence to one).
I recorded a random sequence of 320 utterances of 5 words ("left", "right", "stop", "go", and "?" for background noise) with a sampling rate of 16kHz.
The audio data was split into chunks of 512 samples.
The word start and end were identified using a rising and a falling threshold for the root-mean-square value of each chunk.
The logarithm of the Fourier spectrum of each chunk was used as input for the LSTM.
The initial state and output of the LSTM (16 units each) was set to zero.
The final output state of the LSTM was used as input for a fully connected layer with 5 output units followed by a softmax classifier.
The network was trained in a sequence-to-one setting to recognise a word given a sequence of audio chunks as shown in the following figure.

![Speech recognition](/pics/speech.svg)

The example was trained using gradient descent with multiplier alpha = 0.001.
The training took seven hours on a CPU.
The sequence-to-one setting handles words of different lengths gracefully.
The resulting example is more minimalistic than the [Tensorflow audio recognition example][2].

In the following video the speech recognition is used to control a robot.

{% youtube "https://www.youtube.com/watch?v=xufw3JYK3U8" %}

[1]: https://en.wikipedia.org/wiki/Long_short-term_memory
[2]: https://www.tensorflow.org/tutorials/sequences/audio_recognition
