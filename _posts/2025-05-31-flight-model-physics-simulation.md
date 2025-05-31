---
layout: post
title: Flight model physics for simulation (WIP)
category: simulation
image: /pics/flight.jpg
---

![sfsim space flight simulator screenshot](/pics/flight.jpg)

The dynamic pressure depends on air density and speed:
{% latex usepackages=amsmath %}
$q = \cfrac{1}{2}\,\rho\,V^2$
{% endlatex %}

Air density (and temperature) as a function of height can be obtained from Hull's book "Fundamentals of airplane flight mechanics".

Drag consists of zero-lift drag and induced drag:
{% latex %}
$D_{total} = D_0 + D_i$
{% endlatex %}

The formula for zero-lift drag is:
{% latex %}
$D_0 = C_{D_0}\,q\,S$
{% endlatex %}

The formula for induced drag is
{% latex usepackages=amsmath %}
$D_i = \cfrac{C_L^2}{\pi\,e\,AR}\,q\,S$
{% endlatex %}

<!-- Oswald factor e -->
<!-- Aspect ratio AR -->
<!-- Reference area S -->
<!-- zero-drag coefficient C_{D_0} -->
<!-- C_L lift -->

[1]: https://www.jakobmaier.at/posts/flight-simulation/
