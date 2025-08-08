---
layout: post
title: Orbits with Jolt Physics
category: simulation
image: /pics/aspline.png
---

I want to simulate an orbiting spacecraft using the Jolt Physics engine.

The Jolt Physics engine uses the semi-implicit Euler method to integrate acceleration and speed over time:

{% latex usepackages=amsmath %}
\begin{align*}
\mathbf{v}_{n+1} &= \mathbf{v}_n + \Delta t \mathbf{a}_n\\
\mathbf{x}_{n+1} &= \mathbf{x}_n + \Delta t \mathbf{v}_{n+1}
\end{align*}
{% endlatex %}

The gravitational acceleration by a planet is given by:

{% latex usepackages=amsmath %}
$\mathbf{a}_n = -\cfrac{G\,M}{|\mathbf{x}_n|^3} \mathbf{x}_n$
{% endlatex %}

To test orbiting, one can set the initial conditions of the spacecraft to a perfect circular orbit:

{% latex usepackages=amsmath %}
\begin{align*}
\mathbf{x}_0 &= (R, 0, 0)^\top\\
\mathbf{v}_0 &= (0, V, 0)^\top \mathrm{\ where\ } V = \sqrt{\cfrac{GM}{R}}
\end{align*}
{% endlatex %}

The orbital radius R was set to the Earth radius of 6378 km plus 408 km (the height of the ISS).
The Earth mass was assumed to be 5.9722e+24 kg.

A full orbit was simulated using different values for the time step.
The following plot shows the height deviation from the initial orbital height over time.

![Orbits with symplectic Euler](/pics/euler-height.png)

The good news is that for small time steps (e.g. 30 fps) the accuracy seems to be sufficiently high.

One can show the maximum deviation from the circular orbit as a function of the time step.

![Euler orbit deviation as a function of time step](/pics/euler-errors.png)

For time lapse simulation with a time step of 16 seconds, the errors will exceed 50 km.
A possible solution might be to use Runge Kutta 4th order integration instead of symplectic Euler.
The Jolt Physics library allows to apply impulses to the spacecraft.
The idea is to use Runge Kutta 4th order integration to get an accurate estimate of the speed and position of the spacecraft after the next time step.
One can then apply an impulse before running an Euler step so that the position after the Euler step matches the Runge Kutta estimate.
A second impulse then is used after the Euler time step to also make the speed match the Runge Kutta estimate.
The following plot shows the height deviations observed when using Runge Kutta integration.

![Orbits with Runge Kutta 4th order](/pics/rk-height.png)

The plot of maximum deviation shows that the errors are much smaller.

![RK orbit deviation as a function of time step](/pics/rk-errors.png)
