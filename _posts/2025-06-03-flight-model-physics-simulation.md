---
layout: post
title: Flight model physics for simulation (Draft)
category: simulation
image: /pics/flight.jpg
---

![sfsim space flight simulator screenshot](/pics/flight.jpg)

This is an informational post on how to simulate a Venturestar style single-stage-to-orbit space craft.
My dad is an experienced aerodynamics engineer and I asked him to help with making the aerodynamics of a space flight simulator realistic to some extent.
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

The side force (and corresponding coefficient) was considered negligible.

The following drawing shows the body coordinate system (xb, yb, zb) and the wind coordinate system (xw, yw, zw).
The wind system is rotated against the body system so that the speed vector (in a stationary atmosphere) points in positive xw.

![coordinate systems](/pics/windsystem.png)

* A positive lift force points upwards (negative zw) in the wind system.
* The drag force points backwards (negative xw) in the wind system.
* A positive side force points starboard (positive yw) in the wind system.
* Yaw, pitch, and roll moments on the other hand are specified in the body system.

## TODO
* description of coordinate systems, angles, forces, and moments
* moments: Cl\_beta/alf, Cn\_beta, formula for C\_m
* linear ranges, maxima, argmax
* value tables
* damping

[1]: https://www.jakobmaier.at/posts/flight-simulation/
[2]: https://aerostarsolutions.wordpress.com/wp-content/uploads/2011/10/fundmentals_of_airplane_flight_mechanics.pdf
