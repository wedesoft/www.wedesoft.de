---
layout: post
title: Rigid body game physics 3
category: simulation
---

The following article is based on [Hubert Eichner's article on inequality constraints][1].

## Inequality Constraints
Collisions are inequality constraints.
In contrast to a joint, a collision can only create repellent forces and no attracting forces.
Similar to a joint constraint, the collision is represented using a function *C(q(t))*.
Here *C* is one-dimensional and it is basically the distance of the two objects at the contact point.
The inequality constraint of the collision is
{% latex %}
$\dot{C}=Ju+b\ge 0$
{% endlatex %}
Instead of anchor points, one uses vectors *ri* and *rj* from the center of each object to the contact point.
Using the contact normal *n* the inequality constraint becomes:
{% latex %}
$\dot{C}= n^\top (v_i+\omega_i\times r_i-v_j-\omega_j\times r_j)\ge 0$
{% endlatex %}
The rotational component can be simplified as shown below:
{% latex %}
$n^\top (\omega_i \times r_i) = -n^\top (r_i \times \omega_i) = -n^\top r_i^\times \omega_i = r_i^\times n \omega_i = (r_i \times n) \omega_i$
{% endlatex %}
Thus the linear components of *J* are
{% latex %}
$J_{lin}^i = n^\top$, $J_{lin}^j = -n^\top$
{% endlatex %}
And the angular components of *J* are
{% latex %}
$J_{ang}^i = (r_i \times n)^\top$, $J_{ang}^j = -(r_j \times n)^\top$
{% endlatex %}
The correction term depends on the distance *d*.
I.e. if the distance is negative, a correction is required so that the objects do not penetrate each other any more.
{% latex %}
$b = max(-d, 0)$
{% endlatex %}

## Sequential Impulses (updated)
The contact constraints and the joint constraints are updated for multiple iterations in a loop.
The (cumulative) *λ* obtained for contact constraint furthermore is clipped to be greater or equal to zero.
Note that it has to be the cumulative value which is clipped.
The algorithm for solving the joint and contact constraints becomes:

* for each iteration
    * for each joint constraint
        1. compute Jacobian *J* and correction vector *b*
        1. compute *λ*
        1. compute the impulse *P*
        1. add *P* to accumulated impulses of the two objects
    * for each contact constraint
        1. subtract old impulses *P* from previous iteration from accumulated impulses of the two objects
        1. compute Jacobian *J* and correction vector *b*
        1. compute new *λ* and clip it to be greater or equal to zero
        1. compute the impulse *P*
        1. add *P* to accumulated impulses of the two objects
* use impulses to perform numerical integration step

[1]: http://myselph.de/gamePhysics/inequalityConstraints.html
