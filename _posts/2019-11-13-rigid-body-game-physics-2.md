---
layout: post
title: Rigid body game physics 2
category: simulation
---

The following article is heavily based on [Hubert Eichner's article on equality constraints][1].
The math for the joint types was taken from [Kenny Erleben's PhD thesis][2].

## Constraint Impulses
A constraint between two objects is specified as a multi-dimensional function *C(q(t))*.
The constrained is fulfilled by attempting to keep each component of the derivative of *C* at zero:
{% latex %}
$\dot{C}=\displaystyle\frac{\delta C}{\delta q}\frac{\delta q}{\delta t}=Ju=\vec{0}$
{% endlatex %}
*J* is the Jacobian matrix and *u* is a twelve-element vector containing the linear and rotational speed of the two bodies.
The two objects without a connecting joint together have twelve degrees of freedom.
Let's assume that the two objects are connected by a hinge joint.
In this case the function *C* has five dimensions because a hinge only leaves seven degrees of freedom.
The Jacobian *J* has five rows and twelve columns.
In practise an additional vector *b* is used to correct for numerical errors.
{% latex %}
$\dot{C}=Ju+b=\vec{0}$
{% endlatex %}

The twelve-element constraint impulse *P* does not do any work and therefore must be orthogonal to *u*.
{% latex %}
$P^\top u=0$
{% endlatex %}
The rows of *J* span the orthogonal space to *u*. Therefore *P* can be expressed as a linear combination of the rows of *J*
{% latex %}
$P=J^\top \lambda$
{% endlatex %}
where *λ* is a vector with for example five elements in the case of the hinge joint.

Given an initial estimate for *u* and the constraint impulse *P* one can compute the final velocity *u*.
{% latex %}
$u=\bar{u}+M^{-1}P$
{% endlatex %}
*M* is the twelve by twelve generalised mass matrix which contains the mass and inertia tensors of the two objects.
{% latex usepackages=amsmath %}
$M=
\begin{pmatrix}
  diag(m_1) & 0      & \cdots    & 0     \\
  0         & I_1    & \ddots    & \vdots\\
  \vdots    & \ddots & diag(m_2) & 0     \\
  0         & \cdots & 0         & I_2
\end{pmatrix}$
{% endlatex %}
Inserting the equation for *u* into the constraint equation yields
{% latex %}
$\dot{C} = J(\bar{u} + M^{-1} J^\top \lambda)+b=\vec{0}$
{% endlatex %}
This equation can be used to determine *λ*
{% latex %}
$\lambda = -(J M^{-1} J^\top)^{-1} (J\bar{u}+b)$
{% endlatex %}
*λ* then can be used to determine the constraint impulse *P*!
When performing the numerical integration, the external forces (multiplied with the timestep) and the constraint impulse are used
to determine the change in linear and rotational speed of the two bodies.

## Joints
### Ball-in-Socket Joint
The matrix *J* for bodies *i* and *j* and the velocity vector *u* can be split up into the linear and angular parts.
{% latex usepackages=amsmath %}
$Ju=
\begin{pmatrix}J^i_{lin} & J^i_{ang} & J^j_{lin} & J^j_{ang}\end{pmatrix}
\begin{pmatrix}v_i \\ \omega_i \\ v_j \\ \omega_j\end{pmatrix}=\vec{0}$
{% endlatex %}
*v* is the three-dimensional linear speed and *ω* is the rotational speed of the individual object.
At the anchor point of the joint the speed of the two rigid bodies must be the same:
{% latex %}
$v_i+\omega_i\times R(q_i)r^i_{anc}-v_j-\omega_j\times R(q_j)r^j_{anc}=\vec{0}$
{% endlatex %}
Thus the linear components of *J* are
{% latex usepackages=amsmath %}
$J^i_{lin}=\begin{pmatrix} 1 & 0 & 0\\ 0 & 1 & 0\\ 0 & 0 & 1 \end{pmatrix}$,
$J^j_{lin}=\begin{pmatrix} -1 & 0 & 0\\ 0 & -1 & 0\\ 0 & 0 & -1 \end{pmatrix}$
{% endlatex %}
and the angular components of *J* are
{% latex usepackages=amsmath %}
$J^i_{ang}=-(R(q_i)r^i_{anc})^\times$,
$J^j_{ang}=(R(q_j)r^j_{anc})^\times$
{% endlatex %}
where the cross-product matrix of a vector is defined as follows:
{% latex usepackages=amsmath %}
$x^\times = \begin{pmatrix} 0 & -x_3 & x_2 \\ x_3 & 0 & -x_1 \\ -x_2 & x_1 & 0 \end{pmatrix}$
{% endlatex %}
The correcting vector *b* simply corrects for the offset at the anchor point.
{% latex %}
$b = r_i+R(q_i)r_{anc}^i - r_j - R(q_j)r_{anc}^j$
{% endlatex %}

### Hinge Joint
A hinge joint has the constraints of the ball-in-socket joint and two additional angular constraints.
I.e. there are five constraints altogether.
{% latex usepackages=amsmath %}
$J^i_{lin}=\begin{pmatrix} 1 & 0 & 0\\ 0 & 1 & 0\\ 0 & 0 & 1 \\ 0 & 0 & 0 \\ 0 & 0 & 0\end{pmatrix}$,
$J^j_{lin}=\begin{pmatrix} -1 & 0 & 0\\ 0 & -1 & 0\\ 0 & 0 & -1 \\ 0 & 0 & 0 \\ 0 & 0 & 0\end{pmatrix}$
{% endlatex %}
The rotation axis in world coordinates can be computed from the average of the two rotated axes
which should be coinciding under ideal circumstances.
{% latex %}
$s=\frac{1}{2}(R(q_i)s_i+R(q_j)s_j)$
{% endlatex %}

The relative rotation of the two rigid bodies must be parallel to the axis *s* of the hinge.
Given two vectors *t1* and *t2* orthogonal to the rotation axis of the hinge joint,
the two additional constraints are:
{% latex %}
$t_1 (\omega_i - \omega_j) = 0$ and $t_2 (\omega_i - \omega_j) = 0$
{% endlatex %}
Therefore the angular parts of the Jacobian are
{% latex usepackages=amsmath %}
$J^i_{ang}=\begin{pmatrix} -(R(q_i)r^i_{anc})^\times \\ t_1^\top \\ t_2^\top \end{pmatrix}$,
$J^j_{ang}=\begin{pmatrix} (R(q_j)r^j_{anc})^\times \\ -t_1^\top \\ -t_2^\top \end{pmatrix}$
{% endlatex %}

The error *u* of axis misalignment is the cross product of the two rotated axes.
{% latex %}
$u=(R(q_i)s_i)\times (R(q_j)s_j)$
{% endlatex %}
The error *u* is orthogonal to the rotation axis *s*.
It is projected onto the vectors *t1* and *t2* when computing the correction vector *b*:
{% latex usepackages=amsmath %}
$b=\begin{pmatrix} r_i+R(q_i)r_{anc}^i - r_j - R(q_j)r_{anc}^j\\ t_1 u \\ t_2 u \end{pmatrix}$
{% endlatex %}

See [Kenny Erleben's PhD thesis][2] for these and other types of joints
(slider joint, hinge-2 joint, universal joint, fixed joint).

## Sequential Impulses
To fulfill multiple constraints, an algorithm called *sequential impulses* is used.
Basically the constraint impulses are updated a few times until the impulses become stable.

* for each iteration
    * for each constraint
        1. compute Jacobian *J* and correction vector *b*
        1. compute the impulse *P*
        1. add *P* to accumulated impulse of the two objects
* use impulses to perform numerical integration step

The sequential impulse computation has to be performed four times when using the Runge-Kutta method.

[1]: http://myselph.de/gamePhysics/equalityConstraints.html
[2]: http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
