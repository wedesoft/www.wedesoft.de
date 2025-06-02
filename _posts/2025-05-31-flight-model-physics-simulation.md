---
layout: post
title: Flight model physics for simulation (WIP)
category: simulation
image: /pics/flight.jpg
---

![sfsim space flight simulator screenshot](/pics/flight.jpg)

**this article is currently a work in progress**

This is an informational post on how to simulate a Venturestar style single-stage-to-orbit space craft.
My dad is an experienced aerodynamics engineer and I asked him to help with making the aerodynamics of my space flight simulator realistic to some extent.
The information in this post is a write-up of relevant formulas and approximate data he obtained from numerical simulation and estimates from aerodynamics knowledge.

The dynamic pressure *q* depends on air density *ρ* and speed *V*:
{% latex usepackages=amsmath %}
$q = \cfrac{1}{2}\,\rho\,V^2$
{% endlatex %}

Air density (and temperature) as a function of height can be obtained from [Hull's book "Fundamentals of airplane flight mechanics"][2].

Drag consists of zero-lift drag and induced drag:
{% latex %}
$D_{total} = D_0 + D_i$
{% endlatex %}

Zero-lift drag is computed using the zero-lift drag coefficient as well as dynamic pressure *q* and the reference area:
{% latex %}
$D_0 = C_{D_0}\,q\,S_{ref}$
{% endlatex %}
The zero-lift drag coefficient depends on the speed of the aircraft.

Induced drag is determined using the lift coefficient, the Oswald factor *e*, the aspect ratio *Λ*, as well as *q* and the reference area.
{% latex usepackages=amsmath,txfonts %}
$D_i = \underbrace{\cfrac{C_L^2}{\pi\,e\,\Lambda}}_{\eqqcolon C_{D_i}}\,q\,S_{ref}$
{% endlatex %}
The Oswald factor *e* depends on the speed of the aircraft.
The lift coefficient depends on the angle of attack

The aspect ratio *Λ* depends on wing span *b* and wing area *S*:
{% latex usepackages=amsmath %}
$\Lambda = \cfrac{b^2}{S}$
{% endlatex %}

The lift *L* is computed using the lift coefficient, dynamic pressure *q*, and the reference area:
{% latex usepackages=amsmath %}
$L = C_L\,q\,S_{ref}$
{% endlatex %}

<!-- side force Y and coefficient C_Y -->
<!-- coordinate system aircraft -->
<!-- wind system and angles -->
<!-- Cl_beta/alf -->
<!-- damping -->

[1]: https://www.jakobmaier.at/posts/flight-simulation/
[2]: https://aerostarsolutions.wordpress.com/wp-content/uploads/2011/10/fundmentals_of_airplane_flight_mechanics.pdf
