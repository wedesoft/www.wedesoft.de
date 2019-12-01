---
layout: post
title: Rigid body game physics
category: simulation
---

<a href="/simulation/2019/10/24/rigid-body-game-physics/">part 1</a>
<a href="/simulation/2019/11/13/rigid-body-game-physics-2/">part 2</a>
<a href="/simulation/2019/11/25/rigid-body-game-physics-3/">part 3</a>
<a href="/simulation/2019/11/29/rigid-body-game-physics-4/">part 4</a>
<a href="/simulation/2019/12/01/rigid-body-game-physics-5/">part 5</a>

Inspired by [Hubert Eichner's articles on game physics][1] I decided to write about game physics as well.
My motivation is to create a space flight simulator.
The idea is to simulate a space ship with landing gear and docking adapter using multiple rigid bodies.

## The Newton-Euler equation
Each rigid body has a center with the 3D position *c*.
The orientation of the object can be described using a quaternion *q* which has four components.
The speed of the object is a 3D vector *v*.
Finally the 3D vector *ω* specifies the current rotational speed of the rigid body.

The time derivatives (indicated by a dot on top of the variable name) of the position and the orientation are ([Sauer et al.][2]):
{% latex %}
$\dot{c} = v$, $\dot{q} = \frac{1}{2} \omega q$
{% endlatex %}
The multiplication is a quaternion multiplication where the rotational speed *ω* is converted to a quaternion:
{% latex %}
$\omega = 0 + \omega_1 i + \omega_2 j + \omega_3 k$
{% endlatex %}

In a given time step *Δt* the position changes as follows
{% latex %}
$c^{t+\Delta t} = c^t + v \Delta t$
{% endlatex %}

The orientation *q* changes as shown below
{% latex %}
$q^{t+\Delta t} = q^t + \frac{1}{2} \omega q \Delta t$
{% endlatex %}
Note that *ω* again is converted to a quaternion.

Given the mass *m* and the sum of external forces *F* the speed changes like this
{% latex %}
$v^{t+\Delta t} = v^t + \frac{1}{m} F \Delta t$
{% endlatex %}
In other words this means
{% latex %}
$\dot{v} = \frac{1}{m} F$
{% endlatex %}

Finally given the rotated inertia matrix *I(t)* and the overall torque *τ* one can determine the change of rotational speed
{% latex %}
$\omega^{t+\Delta t} = \omega^t + I(t)^{-1} (\tau - \omega \times I(t) \omega) \Delta t$
{% endlatex %}
Or written as a differential equation
{% latex %}
$\dot{\omega} = I(t)^{-1} (\tau - \omega \times I(t) \omega)$
{% endlatex %}

These are the [Newton-Euler equations][3].
The "x" is the vector cross product.
Note that even if the overall torque *τ* is zero, the rotation vector still can change over time if the inertial matrix *I* has different eigenvalues.

The rotated inertia matrix is obtained by converting the quaternion *q* to a rotation matrix *R* and using it to rotate *I₀*:
{% latex %}
$I(t)=R(q^t) I_0 R(q^t)^\top$
{% endlatex %}

The three column vectors of *R* can be determined by performing quaternion rotations on the unit vectors.
{% latex usepackages=amsmath %}
$R(q) = \big(
q \begin{pmatrix}1 \\ 0 \\ 0 \end{pmatrix} \bar{q},
q \begin{pmatrix}0 \\ 1 \\ 0 \end{pmatrix} \bar{q},
q \begin{pmatrix}0 \\ 0 \\ 1 \end{pmatrix} \bar{q}
\big)$
{% endlatex %}
Note that the vectors get converted to quaternion and back implicitely.

According to David Hammen, the Newton-Euler equation [can be used unmodified in the world inertial frame][5].

## The Runge-Kutta Method
The different properties of the rigid body can be stacked in a state vector *y* as follows.
{% latex usepackages=amsmath %}
\setcounter{MaxMatrixCols}{20}
$y = \begin{pmatrix}c_1 & c_2 & c_3 & q_1 & q_2 & q_3 & q_4 & v_1 & v_2 & v_3 & \omega_1 & \omega_2 & \omega_3\end{pmatrix}^\top$
{% endlatex %}
The equations above then can be brought into the following form
{% latex %}
$\dot{y} = f(t, y)$
{% endlatex %}
Using *h=Δt* the numerical integration for a time step can be performed using the [Runge-Kutta method][4]:
{% latex usepackages=amsmath %}
\begin{equation*}
  \begin{split}
    y^{t+h} &= y^t + \frac{1}{6} (k_1 + 2 k_2 + 2 k_3 + k_4)\\
    k_1 &= h f(t, y^t)\\
    k_2 &= h f(t + \frac{h}{2}, y^t + \frac{k_1}{2})\\
    k_3 &= h f(t + \frac{h}{2}, y^t + \frac{k_2}{2})\\
    k_4 &= h f(t + h, y^t + k_3)
  \end{split}
\end{equation*}
{% endlatex %}

The Runge-Kutta algorithm can also be formulated using a function which instead of derivatives returns infitesimal changes
{% latex %}
$y^{t+h} - y^t = f(t, h, y)$
{% endlatex %}

The Runge-Kutta formula then becomes
{% latex usepackages=amsmath %}
\begin{equation*}
  \begin{split}
    y^{t+h} &= y^t + \frac{1}{6} (k_1 + 2 k_2 + 2 k_3 + k_4)\\
    k_1 &= f(t, h, y^t)\\
    k_2 &= f(t + \frac{h}{2}, h, y^t + \frac{k_1}{2})\\
    k_3 &= f(t + \frac{h}{2}, h, y^t + \frac{k_2}{2})\\
    k_4 &= f(t + h, h, y^t + k_3)
  \end{split}
\end{equation*}
{% endlatex %}

Using time-stepping with *F=0* and *τ=0* one can simulate an object tumbling in space.

![Tumble](/pics/tumble.gif)

[1]: http://myselph.de/gamePhysics/
[2]: https://people.mpi-inf.mpg.de/~schoemer/publications/VRST98.pdf
[3]: https://en.wikipedia.org/wiki/Newton%E2%80%93Euler_equations
[4]: https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
[5]: https://physics.stackexchange.com/questions/412181/eulers-equation-for-rigid-body-rotation-applied-to-inertia-frame
