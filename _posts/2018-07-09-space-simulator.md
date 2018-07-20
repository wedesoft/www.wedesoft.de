---
layout: post
title: Steps towards a space simulator
category: simulation
---

I am quite interested in how simulators such as the [Orbiter space simulator][3] are implemented.
A spacecraft can be seen as a rigid object with a [moments of inertia][4] tensor.
Without any forces acting on the object, the rotational moment of the object does not change.
In general the moments of inertia tensor causes the direction of the rotation vector to be different at each point in time even if the rotational moment is not changing.
This motion can be numerically simulated using a higher order integration method such as [4th order Runge-Kutta][1].
Here is a video showing the resulting simulation of a cuboid tumbling in space:

{% youtube "https://www.youtube.com/watch?v=haQ-HNBmahw" %}

[Brian Vincent Mirtich's thesis][2] demonstrates how to simulate collisions of two convex polyhedra.
Furthermore micro-collisions are used as a simple but powerful method to simulate resting contacts.
If the micro-collisions are sufficietly small, a resting object can be approximated with sufficient accuracy:

{% youtube "https://www.youtube.com/watch?v=QhF_altKvBU" %}

One still needs to implement friction (also shown in Mirtich's thesis) which requires a numerical integral to compute the friction occuring during a micro-collision.
Collisions of polyhedra are demonstrated in Mirtich's thesis as well, however it might be simpler to make use of the [GJK algorithm][5].
Planetary bodies, spacecraft, and other non-convex objects could be handled by dividing them into multiple convex objects.
It would also be interesting to integrate soft body physics as shown in [Rigs of Rods][6].
However the accuracy of Rigs of Rods is not sufficiently high for space simulation.
*E.g.* an object tumbling in space would not preserve its momentum.

#### Update:

In the following examples, dynamic Coloumb friction with the ground is simulated.

{% youtube "https://www.youtube.com/watch?v=hOupSx5l4lA" %}

{% youtube "https://www.youtube.com/watch?v=6sYDN3NfjwI" %}

[1]: https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
[2]: https://people.eecs.berkeley.edu/~jfc/mirtich/thesis/mirtichThesis.pdf
[3]: http://orbit.medphys.ucl.ac.uk/
[4]: https://en.wikipedia.org/wiki/List_of_moments_of_inertia
[5]: http://realtimecollisiondetection.net/pubs/SIGGRAPH04_Ericson_GJK_notes.pdf
[6]: https://www.rigsofrods.org/
