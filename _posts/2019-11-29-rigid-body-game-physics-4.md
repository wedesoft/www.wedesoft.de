---
layout: post
title: Rigid body game physics 4
category: simulation
---

<a href="/simulation/2019/10/24/rigid-body-game-physics/">part 1</a>
<a href="/simulation/2019/11/13/rigid-body-game-physics-2/">part 2</a>
<a href="/simulation/2019/11/25/rigid-body-game-physics-3/">part 3</a>
<a href="/simulation/2019/11/29/rigid-body-game-physics-4/">part 4</a>
<a href="/simulation/2019/12/01/rigid-body-game-physics-5/">part 5</a>
<a href="/simulation/2019/12/03/rigid-body-game-physics-6/">part 6</a>

The following article is partially based on [Hubert Eichner's article on inequality constraints][1].

## Inequality Constraints for Colliding Contacts
At a colliding contact the relative speed of the two objects is negative.
I.e. a contact is a colliding (and not a resting) contact if the normal speed at the contact point is below a small negative threshold.
Colliding contacts have restitution, i.e. the collision is partially elastic and the two objects will separate after the collision.
The colliding contacts and affected joints are handled using a special time step of zero duration.
Again the inequality constraint is
{% latex %}
$\dot{C}=Ju+b\ge 0$
{% endlatex %}
The linear and angular components of *J* are the same as for a resting contact.
The correction term *b* however depends on the initial normal speed at the contact point *vn* and the restitution coefficient *ε*:
{% latex %}
$b = \min(\epsilon v_n, 0) $
{% endlatex %}

## Sequential Impulses for Collisions
Again the contact and joint impulses are estimated iteratively.
Note that the normal speed *vn* at each contact is determined beforehand and stays constant during the iterations.

* determine *vn* for all colliding contacts
* for each iteration
    * for each joint
        1. compute Jacobian *J* and correction vector *b*
        1. predict speed *u*
        1. compute *λ*
        1. compute the impulse *P*
        1. add *P* to accumulated impulses of the two objects
    * for each colliding contact
        1. subtract old impulses *P* from previous iteration from accumulated impulses of the two objects
        1. compute Jacobian *J* and correction vector *b*
        1. predict speed *u*
        1. compute new *λ* and clip it to be greater or equal to zero
        1. compute the impulse *P*
        1. add *P* to accumulated impulses of the two objects
* apply impulses to objects

The time step here is zero!
Therefore external forces do not need to be considered while handling collisions.
An object falling to the floor will experience several collisions until the linear and angular speed has decreased sufficiently for the contacts to become resting contacts.

![Collision](/pics/collision.gif)

[1]: http://myselph.de/gamePhysics/inequalityConstraints.html
