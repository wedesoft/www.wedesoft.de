---
layout: post
title: Flight model physics for simulating Venturestar style spacecraft (Draft)
category: simulation
image: /pics/flight.jpg
---

![sfsim space flight simulator screenshot](/pics/flight.jpg)

This is an informational post on how to simulate atmospheric flight of a [Venturestar][3] style single-stage-to-orbit space craft.
My dad is an experienced aerodynamics engineer and I asked him to help with making the aerodynamics of the [sfsim][4] space flight simulator realistic to some extent.
The information in this post is a write-up of relevant formulas and approximate data he obtained from numerical simulation and estimates from aerodynamics knowledge.

## Coordinate systems

The following drawing shows the body coordinate system (xb, yb, zb) and the wind coordinate system (xw, yw, zw).
The wind system is rotated against the body system so that the speed vector (in a stationary atmosphere) points in positive xw.

![coordinate systems](/pics/windsystem.png)

Note that lift, drag, and side force are defined in the wind system and not in the body system.
* A positive lift force points upwards (negative zw) in the wind system.
* The drag force points backwards (negative xw) in the wind system.
* A positive side force points starboard (positive yw) in the wind system.

Yaw, pitch, and roll moments on the other hand are specified in the body system.

A coordinate system transformation from body system to wind system can be performed using two angles:
* *α* is the angle of attack
* *β* is the sideslip angle

When transforming coordinates from body system to wind system, one first rotates by *β* (sideslip angle) about the body z axis (zb).
Then one rotates by *α* (angle of attack) about the new y axis.

## Dynamic pressure

The dynamic pressure *q* depends on air density *ρ* and speed *V*:
{% latex usepackages=amsmath %}
$q = \cfrac{1}{2}\,\rho\,V^2$
{% endlatex %}

Air density (and temperature) as a function of height can be obtained from [Hull's book "Fundamentals of airplane flight mechanics"][2].

## Forces

Drag consists of zero-lift drag and induced drag:
{% latex %}
$D_{total} = D_0 + D_i$
{% endlatex %}

Zero-lift drag is computed using the zero-lift drag coefficient *CD0* as well as dynamic pressure *q* and the reference area *Sref*:
{% latex %}
$D_0 = C_{D_0}\,q\,S_{ref}$
{% endlatex %}
The zero-lift drag coefficient depends on the speed of the aircraft.

Induced drag is determined using the lift coefficient *CL*, the Oswald factor *e*, the aspect ratio *Λ*, as well as *q* and the reference area *Sref*.
{% latex usepackages=amsmath,txfonts %}
$D_i = \underbrace{\cfrac{C_L^2}{\pi\,e\,\Lambda}}_{\eqqcolon C_{D_i}}\,q\,S_{ref}$
{% endlatex %}
The Oswald factor *e* depends on the speed of the aircraft.
The lift coefficient depends on the angle of attack *α*.

The aspect ratio *Λ* depends on wing span *b* and wing area *S*:
{% latex usepackages=amsmath %}
$\Lambda = \cfrac{b^2}{S}$
{% endlatex %}

The lift *L* is computed using the lift coefficient *CL*, dynamic pressure *q*, and the reference area *Sref*:
{% latex usepackages=amsmath %}
$L = C_L\,q\,S_{ref}$
{% endlatex %}

The side force *Y* (and corresponding coefficient) is usually not important but we will look into it later.

## Moments

The pitching moment *M* is computed using the pitching moment coefficient *Cm*, the dynamic pressure *q*, the reference area *Sref*, and the aerodynamic chord *cbar*:
{% latex usepackages=amsmath %}
$M = C_m\,q\,S_{ref}\,c_{bar}$
{% endlatex %}
The pitching moment coefficient depends on the lift coefficient *CL*, the position of the neutral point *XN*, the centre of gravity *xref*. and the aerodynamic chord *cbar*:
{% latex usepackages=amsmath %}
$C_m = C_L\,\cfrac{X_N - x_{ref}}{c_{bar}}$
{% endlatex %}

The yawing moment *N* is the product of the yawing moment coefficient *Cn*, the dynamic pressure *q*, the reference area *Sref*, and half the wing span *b*:
{% latex usepackages=amsmath %}
$N = C_n\,q\,S_{ref}\,\cfrac{b}{2}$
{% endlatex %}
The yawing moment coefficient depends on the side slip angle *β*.

The rolling moment *L* (using the same symbol as lift for some reason) is the product of the rolling moment coefficient *Cl*, the dynamic pressure *q*, the reference area *Sref*, and half the wing span *b*:
{% latex usepackages=amsmath %}
$L = C_l\,q\,S_{ref}\,\cfrac{b}{2}$
{% endlatex %}
The rolling moment coefficient depends on the angle of attack *α* and the side slip angle *β*.

## Data Tables

Here is a data table with information for determining the remaining coefficients depending on the airspeed in Mach (Ma).
The table shows for each speed:
* a factor to determine the lift coefficient *CL*
* the position *XN* of the neutral point
* the Oswald factor *e*
* a factor to determine the rolling moment coefficient *Cl*
* a factor to determine the yawing moment coefficient *Cn*
* the zero-lift drag coefficient *CD0*

{% latex usepackages=booktabs %}
\begin{tabular}{rrrrrrr}\toprule
V/Ma & $C_{L,\alpha}$ & $X_N$   & $e$    & $C_{l,\beta\alpha}$ & $C_{n,\beta}$ & $C_{D_0}$\\\midrule
 0.0 &         2.5596 & 25.7408 & 0.9846 &              -2.9217 &       -0.0134 &   0.04780\\
 0.6 &         2.7825 & 25.8613 & 0.9859 &              -3.1333 &       -0.0067 &   0.04741\\
 0.8 &         3.0453 & 26.0130 & 0.9873 &              -3.3667 &        0.0005 &   0.04728\\\midrule
 1.2 &         2.8237 & 26.3814 & 0.3359 &               0.5971 &        0.3199 &   0.26410\\
 1.4 &         2.7156 & 26.2970 & 0.3167 &              -0.8761 &        0.1797 &   0.26382\\
 1.6 &         2.3735 & 26.0632 & 0.2817 &              -0.0015 &        0.1164 &   0.26355\\
 1.8 &         2.1063 & 25.8788 & 0.2504 &              -0.1113 &        0.0782 &   0.26327\\
 2.0 &         1.8934 & 25.7365 & 0.2255 &              -0.0751 &        0.0521 &   0.26299\\
 3.0 &         1.3273 & 25.4226 & 0.1579 &              -0.1459 &       -0.0123 &   0.20472\\
 4.0 &         0.9907 & 25.2999 & 0.1179 &               0.0981 &       -0.0290 &   0.15512\\
 5.0 &         0.7816 & 25.2361 & 0.0930 &               0.0463 &       -0.0290 &   0.13197\\\midrule
10.0 &         2.0000 & 50.0000 & 0.0006 &               0.0000 &        0.0000 &   0.21126\\\bottomrule
\end{tabular}
{% endlatex %}

For small values of *α*, the lift coefficient increases linearly with *α*:
{% latex %}
$C_L = C_{L,\alpha}\,\alpha$
{% endlatex %}

For small values of *α* and *β*, the rolling moment coefficient increases linearly with the product of *α* and *β*:
{% latex %}
$C_l = C_{l,\beta\alpha}\,\alpha\,\beta$
{% endlatex %}

For small values of *β*, the yawing moment coefficient increases linearly with *β*:
{% latex %}
$C_n = C_{n,\beta}\,\beta$
{% endlatex %}

The following table shows for each speed:
* the value for *α* at which the linear relation of *CL* and *α* breaks down
* the maximum value of *CL*
* the angle of attack where *CL* reaches its maximum
* the drag coefficient for 90° angle of attack

{% latex usepackages=booktabs,gensymb %}
\begin{tabular}{rrrrr}\toprule
V/Ma & $\alpha_{\mathrm{lin}}/\degree$ & $C_{L_{\mathrm{max}}}$ & $\alpha_{C_{L_{\mathrm{max}}}}$ & $C_{D_{90}}$ \\\midrule
 0.0 &                              20 &                   1.20 &                              35 &   1.1000\\
 0.6 &                              22 &                   1.30 &                              33 &   1.1000\\
 0.8 &                              24 &                   1.40 &                              30 &   1.1000\\\midrule
 1.2 &                              24 &                   1.20 &                              30 &   0.0000\\
 1.4 &                              24 &                   1.10 &                              30 &  -0.7289\\
 1.6 &                              24 &                   1.00 &                              30 &  -0.5580\\
 1.8 &                              24 &                   0.90 &                              30 &  -0.4409\\
 2.0 &                              24 &                   0.80 &                              30 &  -0.3571\\
 3.0 &                              24 &                   0.50 &                              30 &  -0.1587\\
 4.0 &                              24 &                   0.40 &                              30 &  -0.0893\\
 5.0 &                              24 &                   0.35 &                              30 &  -0.0571\\\midrule
10.0 &                              30 &                   1.00 &                              45 &  -0.0143\\\bottomrule
\end{tabular}
{% endlatex %}

## TODO
* rudder, flap, aileron
* damping

[1]: https://www.jakobmaier.at/posts/flight-simulation/
[2]: https://aerostarsolutions.wordpress.com/wp-content/uploads/2011/10/fundmentals_of_airplane_flight_mechanics.pdf
[3]: https://en.wikipedia.org/wiki/VentureStar
[4]: https://wedesoft.github.io/sfsim/
