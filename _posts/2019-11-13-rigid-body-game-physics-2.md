---
layout: post
title: Rigid body game physics 2
category: simulation
---

The following article is heavily based on [Hubert Eichner's article on equality constraints][1] and concrete examples for joint types
can be found in [Kenny Erleben's PhD thesis][2].

## Constraint Impulses
A constraint between two objects is specified as a multi-dimensional function *C(q(t))*.
The constrained is fulfilled by attempting to keep each component of the derivative of *C* at zero:
{% latex %}
$\dot{C}=\displaystyle\frac{\delta C}{\delta q}\frac{\delta q}{\delta t}=Jv=\vec{0}$
{% endlatex %}
*J* is the Jacobian matrix and *v* is a twelve-element vector containing the linear and rotational speed of the two bodies.
The two objects together have twelve degrees of freedom.
For example a hinge joint has a five-dimensional function *C* because a hinge only leaves seven degrees of freedom.
In this case the Jacobian *J* has five rows and twelve columns.
In practise an additional vector *b* is used to correct for numerical errors.
{% latex %}
$\dot{C}=Jv+b=\vec{0}$
{% endlatex %}

The twelve-element constraint impulse *P* does not do any work and is orthogonal to *v*.
{% latex %}
$P^\top v=0$
{% endlatex %}
The rows of *J* span the orthogonal space to *v*. Therefore *P* can be expressed as a linear combination of the rows of *J*
{% latex %}
$P=J^\top \lambda$
{% endlatex %}
where *λ* is a vector with for example five elements in the case of the hinge joint.

Given an initial estimate for *v* and the constraint impulse *P* one can compute the final velocity *v*.
{% latex %}
$v=\bar{v}+M^{-1}P$
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
Inserting the equation for *v* into the constraint equation yields
{% latex %}
$\dot{C} = J(\bar{v} + M^{-1} J^\top \lambda)+b=\vec{0}$
{% endlatex %}
This equation can be used to determine *λ*
{% latex %}
$\lambda = -(J M^{-1} J^\top)^{-1} (J\bar{v}+b)$
{% endlatex %}
*λ* then can be used to determine the constraint impulse *P*!
When performing the numerical integration, the external forces (multiplied with the timestep) and the constraint impulse are used
to determine the change in linear and rotational speed of the two bodies.

[1]: http://myselph.de/gamePhysics/equalityConstraints.html
[2]: http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
