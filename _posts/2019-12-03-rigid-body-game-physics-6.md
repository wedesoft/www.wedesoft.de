---
layout: post
title: Rigid body game physics 6
category: simulation
---

<a href="/simulation/2019/10/24/rigid-body-game-physics/">part 1</a>
<a href="/simulation/2019/11/13/rigid-body-game-physics-2/">part 2</a>
<a href="/simulation/2019/11/25/rigid-body-game-physics-3/">part 3</a>
<a href="/simulation/2019/11/29/rigid-body-game-physics-4/">part 4</a>
<a href="/simulation/2019/12/01/rigid-body-game-physics-5/">part 5</a>
<a href="/simulation/2019/12/03/rigid-body-game-physics-6/">part 6</a>

A realistic simulation requires the estimation of friction forces.

## Friction Forces
Colliding and resting contacts cause constraint forces as well as friction forces.
So far we have only estimated the constraint forces of colliding and resting contacts.
Constraint forces are in the direction of the contact's normal vector.
Friction forces act tangential to the contact plane (orthogonal to the normal vector).
The inequality constraint is
{% latex %}
$\dot{C}=Ju+b\ge 0$
{% endlatex %}
*J* and *b* are extended to have three rows.
The vectors *t1* and *t2* are chosen so that they are orthogonal to *n* and orthogonal to each other.
Friction forces try to reduce tangential velocities to zero.
I.e. in the case of a colliding contact, *b* is
{% latex usepackages=amsmath %}
$b = \begin{pmatrix} \min(\epsilon v_n, 0) \\ 0 \\ 0 \end{pmatrix}$
{% endlatex %}
In the case of a resting contact, *b* is
{% latex usepackages=amsmath %}
$b = \begin{pmatrix} \min(d, 0) \\ 0 \\ 0 \end{pmatrix}$
{% endlatex %}

The linear components of *J* are
{% latex usepackages=amsmath %}
$J_{lin}^i = \begin{pmatrix} -n^\top \\ t_1^\top \\ t_2^\top \end{pmatrix}$,
$J_{lin}^j = \begin{pmatrix} n^\top \\ -t_1^\top \\ -t_2^\top \end{pmatrix}$
{% endlatex %}
and the angular components of *J* are
{% latex usepackages=amsmath %}
$J_{ang}^i = \begin{pmatrix} -(r_i \times n)^\top \\ (r_i \times t_1)^\top \\ (r_i \times t_2)^\top \end{pmatrix}$,
$J_{ang}^j = \begin{pmatrix} (r_j \times n)^\top \\ -(r_j \times t_1)^\top \\ -(r_j \times t_2)^\top \end{pmatrix}$
{% endlatex %}

As before *λ* is computed
{% latex %}
$\lambda = -(J M^{-1} J^\top)^{-1} (J\bar{u}+b)$
{% endlatex %}
Here *λ* has three elements.
*λ1* is thresholded so that it is greater or equal to zero.
*λ2* and *λ3* are scaled with the same positive factor if necessary so that the length of the 2D vector consisting of *λ2* and *λ3* is smaller than *λ1* multiplied with the friction coefficient *μ*:
{% latex %}
$\lambda_1 \ge 0$,
$\sqrt{\lambda_2^2+\lambda_3^2} \le \mu\lambda_1 $
{% endlatex %}
Basically the vector *λ* needs to reside in the friction cone.

If you have made it this far, you know how to build a small physics engine.

Any feedback, comments, and suggestions are welcome.
