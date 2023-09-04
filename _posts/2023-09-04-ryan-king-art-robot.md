---
layout: post
title: Mechanical Rigging using Ryan King Art's tutorial
category: graphics
image: /pics/robot.jpg
---

After finishing the [Blender Guru donut tutorial][3] I wanted to look into articulated objects.
Initially I looked at [Blender][4] skeletons but then I found [Ryan King Art's tutorial on mechanical rigging][1].

Basically for each rigid object the following steps are executed:
* The 3D cursor is set to a point on the center of the joint axis
* The origin of the rigid object is set to the 3D cursor
* The parent of the rigid object is set to the other object it is connected to via the joint
* All forbidden translation axes and rotation axes are locked

The resulting object then can be animated by defining keyframes for joint angles as shown in [Blender Guru donut tutorial part 11][2].
Rendering the video using the Cycles engine on my 8 core CPU took 3.5 hours.
If you have a GPU supported by Blender this would take much less time.

See below for the resulting video:

<span class="center"><video controls src="/downloads/robot.mp4" width="100%" loop></video></span>

[1]: https://www.youtube.com/watch?v=J1He21vDIfQ
[2]: https://www.youtube.com/watch?v=LMA3S2EGM6U&list=PLjEaoINr3zgFX8ZsChQVQsuDSjEqdWMAD&index=11&t=713s
[3]: https://www.youtube.com/playlist?list=PLjEaoINr3zgFX8ZsChQVQsuDSjEqdWMAD
[4]: https://www.blender.org/
