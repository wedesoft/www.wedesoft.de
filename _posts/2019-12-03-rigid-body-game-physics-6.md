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

**Update:** See <a href="/simulation/2024/09/26/jolt-physics-engine/">Getting started with the Jolt Physics Engine</a>

A realistic simulation requires the estimation of friction forces.

## Friction Forces
Colliding as well as resting contacts cause constraint impulses as well as friction impulses.
So far we have only estimated the constraint impulses of colliding and resting contacts.
Constraint impulses are in the direction of the contact's normal vector.
Friction impulses act tangential to the contact plane (orthogonal to the normal vector).
The inequality constraint is
{% latex %}
$\dot{C}=Ju+b\ge 0$
{% endlatex %}
*J* and *b* have two rows.
The vectors *t1* and *t2* are chosen so that they are orthogonal to the normal vector *n* and orthogonal to each other.
Friction forces try to reduce tangential velocities to zero.
I.e. *b* is
{% latex usepackages=amsmath %}
$b = \begin{pmatrix} 0 \\ 0 \end{pmatrix}$
{% endlatex %}

The linear components of *J* are
{% latex usepackages=amsmath %}
$J_{lin}^i = \begin{pmatrix} t_1^\top \\ t_2^\top \end{pmatrix}$,
$J_{lin}^j = \begin{pmatrix} -t_1^\top \\ -t_2^\top \end{pmatrix}$
{% endlatex %}
and the angular components of *J* are
{% latex usepackages=amsmath %}
$J_{ang}^i = \begin{pmatrix} (r_i \times t_1)^\top \\ (r_i \times t_2)^\top \end{pmatrix}$,
$J_{ang}^j = \begin{pmatrix} -(r_j \times t_1)^\top \\ -(r_j \times t_2)^\top \end{pmatrix}$
{% endlatex %}

As before *λ* is computed
{% latex %}
$\lambda = -(J M^{-1} J^\top)^{-1} (J\bar{u}+b)$
{% endlatex %}
Here *λ* has two elements.
*λ1* and *λ2* are scaled with the same positive factor if necessary so that the length of the 2D vector consisting of *λ1* and *λ2* is smaller than the normal impulse *Pn* multiplied with the friction constant *μ*.
{% latex %}
$\sqrt{\lambda_1^2+\lambda_2^2} \le \mu P_n$
{% endlatex %}
Basically the overall force vector needs to reside in the friction cone.
In a similar fashion as for constraint impulses, friction impulses are subtracted, recomputed, and then added to the accumulated impulses *P* of the two objects, when iterating.

![friction cone](/pics/cone.svg)

If you have made it this far, you know how to build a small physics engine!

Any feedback, comments, and suggestions are welcome.

![dropping box](/pics/drop.gif)

## Further reading

* [K. Erleben: Stable, robust, and versatile multibody dynamics animation](http://image.diku.dk/kenny/download/erleben.05.thesis.pdf)
* [B. Mirtich: Impulse-based dynamics simulation of rigid body systems](http://www.kuffner.org/james/software/dynamics/mirtich/mirtichThesis.pdf)
* [H. Eichner: Rigid Body Game Physics](http://myselph.de/gamePhysics/index.html)
