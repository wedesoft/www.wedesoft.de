---
layout: post
title: Orbits with Jolt Physics
category: simulation
image: /pics/aspline.png
---

I want to simulate an orbiting spacecraft using the Jolt Physics engine.
The Jolt Physics engine solves difficult problems such as gyroscopic forces, collision detection with linear casting, and special solutions for wheeled vehicles with suspension.

The integration method of the Jolt Physics engine is the semi-implicit Euler method.
The following formula shows how speed **v** and position **x** are integrated for each time step:

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
For increased accuracy, the Jolt Physics library was compiled with the option *-DDOUBLE_PRECISION=ON*.

A full orbit was simulated using different values for the time step.
The following plot shows the height deviation from the initial orbital height over time.

![Orbits with symplectic Euler](/pics/euler-height.png)

When examining the data one can see that the integration method returns close to the initial after one orbit.
The orbital error of the Euler integration method looks like a sine wave.
Even for a small timestep of dt = 0.031 s, the maximum orbit deviation is 123.8 m.
The following plot shows that for increasing time steps, the maximum error grows linearly.

![Euler orbit deviation as a function of time step](/pics/euler-errors.png)

For time lapse simulation with a time step of 16 seconds, the errors will exceed 50 km.
A possible solution might be to use Runge Kutta 4th order integration instead of symplectic Euler.
The Jolt Physics library allows to apply impulses to the spacecraft.
The idea is to use Runge Kutta 4th order integration to get an accurate estimate of the speed and position of the spacecraft after the next time step.
One can then apply an impulse before running an Euler step so that the position after the Euler step matches the Runge Kutta estimate.
A second impulse then is used after the Euler time step to also make the speed match the Runge Kutta estimate.
The following plot shows the height deviations observed when using Runge Kutta integration.

![Orbits with Runge Kutta 4th order](/pics/rk-height.png)

The following plot of maximum deviation shows that the errors are much smaller.

![RK orbit deviation as a function of time step](/pics/rk-errors.png)

Although the accuracy of the Runge Kutta matching scheme is higher, a loss of 40 m of height per orbit is undesirable.
Inspecting the Jolt Physics source code reveals that the double-precision setting affects position vectors but is not applied to speed and impulse vectors.
To test whether double precision speed and impulse vectors would increase the accuracy, a test implementation of the semi-implicit Euler method with Runge Kutta matching scheme was used.
The following plot shows that the orbit deviations are now much smaller.

![Orbits with Runge Kutta 4th order and double precision](/pics/rk-double-height.png)

The updated plot of maximum deviation shows that using double precision the error for one orbit is below 1 meter for time steps up to 40 seconds.

![RK with double precision orbit deviation as a function of time step](/pics/rk-double-errors.png)

I am currently looking into building a modified Jolt Physics version which uses double precision for speed and impulse vectors.
I hope that I will get the Runge Kutta 4th order matching scheme to work so that I get an integrated solution for numerically accurate orbits as well as collision and vehicle simulation.
