---
layout: post
title: Flight dynamics model for simulating Venturestar style spacecraft
category: simulation
image: /pics/flight.jpg
---

![sfsim space flight simulator screenshot](/pics/flight.jpg)

This is an informational post on how to simulate the physics of atmospheric flight of a [Venturestar][2] style single-stage-to-orbit space craft.
My dad **Gerhard Wedekind** is an experienced aerodynamics engineer and I asked him to help with making the aerodynamics of the [sfsim][3] space flight simulator realistic to some extent.
The information in this post is a write-up of relevant formulas and approximate data he obtained from numerical simulation and estimates from aerodynamics knowledge.
The information provided in this article is for general informational purposes only and comes without any warranty, express or implied.

## Simulation

Here are a few beautiful snapshots from simulation.
The first one shows a Mach box for V = 2 Ma and α = 3°.
![Venturestar Mach Box for V = 2 Ma and α = 3°](/pics/venturestar-machbox-2-alpha-3.jpg)

The next one shows a Mach box for V = 4 Ma and α = 3°.
![Venturestar Mach Box for V = 4 Ma and α = 3°](/pics/venturestar-machbox-4-alpha-3.jpg)

Finally here is a distribution of the pressure difference between top and bottom of wing.
![Venturestar Pressure Difference](/pics/venturestar-pressure-difference.jpg)

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

Air density (and temperature) as a function of height can be obtained from [Hull's book "Fundamentals of airplane flight mechanics"][1].

## Forces

Drag consists of zero-lift drag and induced drag:
{% latex %}
$D_{total} = D_0 + D_i$
{% endlatex %}

Zero-lift drag is computed using the zero-lift drag coefficient *CD0* as well as dynamic pressure *q* and the reference area *Sref*:
{% latex %}
$D_0 = C_{D_0}\,q\,S_{\mathrm{ref}}$
{% endlatex %}
The zero-lift drag coefficient depends on the speed of the aircraft.

Induced drag is determined using the lift coefficient *CL*, the Oswald factor *e*, the aspect ratio *Λ*, as well as *q* and the reference area *Sref*.
{% latex usepackages=amsmath,txfonts %}
$D_i = \underbrace{\cfrac{C_L^2}{\pi\,e\,\Lambda}}_{\eqqcolon C_{D_i}}\,q\,S_{\mathrm{ref}}$
{% endlatex %}
The Oswald factor *e* depends on the speed of the aircraft.
The lift coefficient depends on the angle of attack *α*.

The aspect ratio *Λ* depends on wing span *b* and wing area *S*:
{% latex usepackages=amsmath %}
$\Lambda = \cfrac{b^2}{S}$
{% endlatex %}

The lift *L* is computed using the lift coefficient *CL*, dynamic pressure *q*, and the reference area *Sref*:
{% latex usepackages=amsmath %}
$L = C_L\,q\,S_{\mathrm{ref}}$
{% endlatex %}

The side force *Y* (and corresponding coefficient) is usually not important but we will look into it <s>later</s> in a future article.

## Moments

The pitching moment *M* is computed using the pitching moment coefficient *Cm*, the dynamic pressure *q*, the reference area *Sref*, and the aerodynamic chord *cbar*:
{% latex usepackages=amsmath %}
$M = C_m\,q\,S_{\mathrm{ref}}\,\bar{c}$
{% endlatex %}
The pitching moment coefficient depends on the lift coefficient *CL*, the position of the neutral point *XN*, the centre of gravity *xref*. and the aerodynamic chord *cbar*:
{% latex usepackages=amsmath %}
$C_m = C_L\,\cfrac{x_{\mathrm{ref} - X_N}}{\bar{c}}$
{% endlatex %}

The yawing moment *N* is the product of the yawing moment coefficient *Cn*, the dynamic pressure *q*, the reference area *Sref*, and half the wing span *b*:
{% latex usepackages=amsmath %}
$N = C_n\,q\,S_{\mathrm{ref}}\,\cfrac{b}{2}$
{% endlatex %}
The yawing moment coefficient depends on the side slip angle *β*.

The rolling moment *L* (using the same symbol as lift for some reason) is the product of the rolling moment coefficient *Cl*, the dynamic pressure *q*, the reference area *Sref*, and half the wing span *b*:
{% latex usepackages=amsmath %}
$L = C_l\,q\,S_{\mathrm{ref}}\,\cfrac{b}{2}$
{% endlatex %}
The rolling moment coefficient depends on the angle of attack *α* and the side slip angle *β*.

## Data Sheet

Here are the parameters for the flight model above:

{% latex usepackages=units %}
\begin{itemize}
\item wing area: $S=\unit[682.1415]{m^2}$
\item reference area: $S_{\mathrm{ref}}=\unit[668.7206]{m^2}$
\item aspect ratio: $\Lambda=2.6226$
\item half span: $b/2=\unit[21.1480]{m}$
\item aerodynamic chord: $\bar{c}=\unit[22.5714]{m}$
\item reference x-station: $x_{\mathrm{ref}}=\unit[23.5473]{m}$
\end{itemize}
{% endlatex %}

Note that *xref* is defined in a coordinate system where *x=0* is at the intersection of the inner leading edges (wing apex).
The following picture also shows the position of the aerodynamic chord with length *cbar*.
The center of gravity is at 25% of the aerodynamic chord.

![coordinate system origin at intersection of leading edges](/pics/leadingedges.jpg)

## Tables

Here is a data table with information for determining the remaining coefficients depending on the airspeed in Mach (Ma).
The table shows for each speed:
* a factor to determine the lift coefficient *CL*
* the position *XN* of the neutral point relative to the aerodynamic chord (note that the center of gravity *xref* is at the 25% mark of the aerodynamic chord)
* the Oswald factor *e*
* a factor to determine the rolling moment coefficient *Cl*
* a factor to determine the yawing moment coefficient *Cn*
* the zero-lift drag coefficient *CD0*

{% latex usepackages=booktabs %}
\begin{tabular}{rrrrrrr}\toprule
V/Ma & $C_{L,\alpha}$ & $(X_N-x_{\mathrm{ref}})/\bar{c} + 0.25 $ & $e$    & $C_{l,\beta/\alpha}$ & $C_{n,\beta,\mathrm{tot}}$ & $C_{D_0}$\\\midrule
 0.0 &         2.5596 & 25.7408\% & 0.9846 &              -2.9217 &                     0.0369 &   0.04780\\
 0.6 &         2.7825 & 25.8613\% & 0.9859 &              -3.1333 &                     0.0578 &   0.04741\\
 0.8 &         3.0453 & 26.0130\% & 0.9873 &              -3.3667 &                     0.0923 &   0.04728\\\midrule
 1.2 &         2.8237 & 26.3814\% & 0.3359 &               0.5971 &                     0.3199 &   0.26410\\
 1.4 &         2.7156 & 26.2970\% & 0.3167 &              -0.8761 &                     0.1797 &   0.26382\\
 1.6 &         2.3735 & 26.0632\% & 0.2817 &              -0.0015 &                     0.1164 &   0.26355\\
 1.8 &         2.1063 & 25.8788\% & 0.2504 &              -0.1113 &                     0.0782 &   0.26327\\
 2.0 &         1.8934 & 25.7365\% & 0.2255 &              -0.0751 &                     0.0521 &   0.26299\\
 3.0 &         1.3273 & 25.4226\% & 0.1579 &              -0.1459 &                    -0.0123 &   0.20472\\
 4.0 &         0.9907 & 25.2999\% & 0.1179 &               0.0981 &                    -0.0398 &   0.15512\\
 5.0 &         0.7816 & 25.2361\% & 0.0930 &               0.0463 &                    -0.0554 &   0.13197\\\midrule
10.0 &         2.0000 & 50.0000\% & 0.0264 &               0.0000 &                    -0.0290 &   0.21126\\\bottomrule
\end{tabular}
{% endlatex %}

The outlier of Clβα for V = 1.2 Ma (0.5971) should be ignored because the value was changing a lot with mesh resolution.

The speed of sound as a function of temperature *T* is [according to Wikipedia][4]:
{% latex usepackages=amsmath %}
$c_{\mathrm{air}} \approx 331.3\,\mathrm{m/s}\,\sqrt{\cfrac{T}{273.15\,\mathrm{K}}}$
{% endlatex %}

For small values of *α*, the lift coefficient increases linearly with *α* (where *α* is specified in radians):
{% latex %}
$C_L = C_{L,\alpha}\,\alpha$
{% endlatex %}

For small values of *α* and *β*, the rolling moment coefficient increases linearly with the product of *α* and *β* (where *α* and *β* are specified in radians):
{% latex %}
$C_l = C_{l,\beta/\alpha}\,\alpha\,\beta$
{% endlatex %}

For small values of *β*, the yawing moment coefficient increases linearly with *β* (where *β* is specified in radians):
{% latex %}
$C_n = C_{n,\beta\,\mathrm{tot}}\,\beta$
{% endlatex %}

The following table shows for each speed:
* the value for *α* at which the linear relation of *CL* and *α* breaks down
* the maximum value of *CL*
* the angle of attack where *CL* reaches its maximum
* the drag coefficient for 90° angle of attack

{% latex usepackages=booktabs,gensymb %}
\begin{tabular}{rrrrr}\toprule
V/Ma & $\alpha_{\mathrm{lin}}/\degree$ & $C_{L_{\mathrm{max}}}$ & $\alpha_{C_{L_{\mathrm{max}}}}/\degree$ & $C_{D_{90}}$ \\\midrule
 0.0 &                              20 &                   1.20 &                              35 &   1,1000\\
 0.6 &                              22 &                   1.30 &                              33 &   1,1000\\
 0.8 &                              24 &                   1.40 &                              30 &   1,1000\\\midrule
 1.2 &                              24 &                   1.20 &                              30 &   2,3884\\
 1.4 &                              24 &                   1.10 &                              30 &   1,4936\\
 1.6 &                              24 &                   1.00 &                              30 &   1,5653\\
 1.8 &                              24 &                   0.90 &                              30 &   1,6180\\
 2.0 &                              24 &                   0.80 &                              30 &   1,6573\\
 3.0 &                              24 &                   0.50 &                              30 &   1,7557\\
 4.0 &                              24 &                   0.40 &                              30 &   1,7918\\
 5.0 &                              24 &                   0.35 &                              30 &   1,8088\\\midrule
10.0 &                              30 &                   1.00 &                              45 &   1,8317\\\bottomrule
\end{tabular}
{% endlatex %}

Near *α=90°*, the lift and drag coefficients behave as follows:
{% latex %}
$C_L=C_{D_{90}}\,\cos(\alpha)$, $C_{D_0}=C_{D_{90}}\,\sin(\alpha)$
{% endlatex %}

At hypersonic speeds (V/Ma=10.0), lift and induced drag coefficients behave as follows:
{% latex %}
$C_L=C_N\,\cos(\alpha)$, $C_{D_i}=C_N\,\sin(\alpha)$, $C_N=2\,\sin(\alpha)$
{% endlatex %}

I.e. the coefficients are stabilising at hypersonic speeds!

## Control surfaces

The following table shows parameters to determine different moments generated by control surfaces:

{% latex usepackages=booktabs %}
\begin{tabular}{rrrrrr}\toprule
& \multicolumn{2}{c}{Rudder} & Flap & \multicolumn{2}{c}{Aileron}\\
V/Ma & $C_{Y \beta R}$ & $C_{n \beta R}$ & $C_{m \delta F}$ & $C_{l \xi A}$ & $C_{n \xi A}$\\\midrule
 0.0 &          0.0798 &         -0.0632 &          -0.3736 &       -0.3566 & 0.0586\\
 0.6 &          0.0875 &         -0.0693 &          -0.4277 &       -0.3935 & 0.0653\\
 0.8 &          0.0966 &         -0.0765 &          -0.4976 &       -0.4379 & 0.0737\\\midrule
 1.2 &          0.1201 &         -0.0951 &          -0.4226 &       -0.3715 & 0.0000\\
 1.4 &          0.0813 &         -0.0644 &          -0.2861 &       -0.2515 & 0.0000\\
 1.6 &          0.0638 &         -0.0505 &          -0.2244 &       -0.1973 & 0.0000\\
 1.8 &          0.0532 &         -0.0421 &          -0.1873 &       -0.1647 & 0.0000\\
 2.0 &          0.0460 &         -0.0364 &          -0.1618 &       -0.1423 & 0.0000\\
 3.0 &          0.0282 &         -0.0201 &          -0.0991 &       -0.0871 & 0.0000\\
 4.0 &          0.0206 &         -0.0147 &          -0.0724 &       -0.0636 & 0.0000\\
 5.0 &          0.0163 &         -0.0116 &          -0.0572 &       -0.0503 & 0.0000\\\midrule
10.0 &          0.0398 &         -0.0284 &          -0.1402 &       -0.1154 & 0.0000\\\bottomrule
\end{tabular}
{% endlatex %}

The side force coefficient for a given rudder angle *ζ* is:
{% latex %}
$C_Y = C_{Y \beta R}\,\zeta$
{% endlatex %}
The yawing moment coefficient for the rudder is:
{% latex %}
$C_n = C_{n \beta R}\,\zeta$
{% endlatex %}

The pitching moment coefficient for flaps *δF* (down is positive) is
{% latex %}
$C_m = C_{m \delta F}\,\delta F$
{% endlatex %}

The rolling moment coefficient for ailerons with angle *ξ* (positive: port aileron up, starboard aileron down) is:
{% latex %}
$C_l = C_{l \xi A}\,\xi$
{% endlatex %}
The yawing moment coefficient is
{% latex %}
$C_n = C_{n \xi A}\,\xi$
{% endlatex %}

## Angular damping

The formula for roll, pitch, and yaw damping moments (L, M, N) due to roll, pitch, and yaw rates (p, q, r) uses a coefficient matrix:
{% latex usepackages=amsmath %}
$\begin{pmatrix}
L \\
M \\
N
\end{pmatrix}
=
\cfrac{1}{2}\,\rho\,V^2\,S
\cdot
\begin{pmatrix}
b & 0 & 0 \\
0 & c & 0 \\
0 & 0 & b
\end{pmatrix}
\cdot
\begin{pmatrix}
C_{l_p} & C_{l_q} & C_{l_r} \\
C_{m_p} & C_{m_q} & C_{m_r} \\
C_{n_p} & C_{n_q} & C_{n_r}
\end{pmatrix}
\cdot
\cfrac{1}{2\,V}
\cdot
\begin{pmatrix}
p\,b \\
q\,c \\
r\,b
\end{pmatrix}$
{% endlatex %}
The coefficients for *V = 0 Ma* are as follows.
Note that damping moments are negligible for higher speeds.
{% latex usepackages=booktabs %}
\begin{tabular}{lr}\toprule
coefficient & value \\\midrule
$C_{l_p}$ & -0.4228\\
$C_{m_p}$ &  0.0000\\
$C_{n_p}$ &  0.0535\\
$C_{l_q}$ &  0.0000\\
$C_{m_q}$ & -1.3470\\
$C_{n_q}$ &  0.0000\\
$C_{l_r}$ &  0.1929\\
$C_{m_r}$ &  0.0000\\
$C_{n_r}$ & -0.2838\\\midrule
$C_{L_q}$ &  4.2624\\\bottomrule
\end{tabular}
{% endlatex %}
The last (10th) value in the table *CLq* is the change in the lift coefficient due to the pitch rate q.

## Next steps

Using the information, the curves for a full range of angles and speeds need to be fitted and guessed in some places.

Feel free to leave a comment or suggestion below.

[1]: https://aerostarsolutions.wordpress.com/wp-content/uploads/2011/10/fundmentals_of_airplane_flight_mechanics.pdf
[2]: https://en.wikipedia.org/wiki/VentureStar
[3]: https://wedesoft.github.io/sfsim/
[4]: https://en.wikipedia.org/wiki/Speed_of_sound
