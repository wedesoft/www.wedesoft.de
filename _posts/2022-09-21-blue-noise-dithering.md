---
layout: post
title: Blue Noise Dithering
category: software
image: /pics/bluenoise.png
---

This is a short description on blue noise dithering.
For more detail see the [1993 paper about blue noise dithering][1].

Initially a binary pattern BP is created where about 10 percent of the pixels are set to true.

![random mask](/pics/random-mask.png)

The binary pattern then is convolved with the following filter function to generate the density array DA:

{% latex %}
$\displaystyle f(x, y) = e ^ {-\frac{x^2+y^2}{2\sigma^2}}$
{% endlatex %}

The convolution is wrapped around to facilitate a tileable result:

{% latex %}
$\displaystyle DA(x, y) = \sum_{p=-M/2}^{M/2} \sum_{q=-M/2}^{M/2} BP(p\ mod\ M, q\ mod\ M) f(p, q)$
{% endlatex %}

Maxima of the density array are called clusters and minima are called voids.

The true value in BP with the highest density value DA (tightest cluster) is set to false and DA is updated accordingly.
Now the false value in BP with the lowest density value DA (largest void) is set to true.

This is repeated until disolving the tightest cluster creates the largest void.

![seed](/pics/seed.gif)

[1]: http://cv.ulichney.com/papers/1993-void-cluster.pdf
