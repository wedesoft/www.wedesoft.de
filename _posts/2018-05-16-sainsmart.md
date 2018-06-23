---
layout: post
title: Sainsmart 6-dof robot arm
image: /pics/sainsmart.jpg
---

[Sainsmart is selling a 6-dof robot arm][1].
One can control the robot arm using an Arduino controller and a DFRobot I/O expansion shield.
I mounted a [Sunfounder Rollpaw][3] gripper on the robot (note that the servo shaft is not compatible with the Sainsmart servos and I had to replace the wrist servo of the Sainsmart robot).
I developed some [software][2] to perform smooth and synchronized motion with the robot drives.
The robot can be controlled using a serial terminal.
Alternatively one can use an XBox controller.
The software performing inverse kinematics is demonstrated in the following video.

{% youtube "http://www.youtube.com/watch?v=ufObK9-eSjE" %}

**See also:**

* [Sainsmart 6-dof robot arm][1]
* [Software repository and part list][2]

[1]: https://www.sainsmart.com/products/6-axis-desktop-robotic-arm-assembled
[2]: https://github.com/wedesoft/arduino-sainsmart
[3]: https://www.sunfounder.com/rollpaw.html
