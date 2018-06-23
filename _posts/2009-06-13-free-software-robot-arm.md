---
layout: post
title: Free software robotic arm
image: /pics/katana_tux.jpg
---
Industrial robotics still is dominated by proprietary "off-the-shelf"
solutions. It is very common to find that a single robot is controlled by
several unrelated software products which do not interoperate with each other.
Furthermore the control PCs are running Windows XP which is scheduled using a
hardware task switcher and the VxWorks software. Basically the situation is
a set of hacks which reflects the political situation of the marketplace
rather than the demands of the technical problem at hand.

The situation is very reminiscent of medieval guilds. The medieval guilds
were small communities (companies) where an apprentice had to stay with a
master for many years to get inaugurated into the secrets (intellectual
property) of the trade. The career options were extremely limited since the
guilds had the exclusive right of selling products on the market (OEM deals).
After many centuries of moderate progress, industrial mass production and global
trading finally swept away the rigid guild system. But I am digressing.

<span class="right"><a href="http://www.neuronics.ch/cms_de/web/index.php?identifier=linux_robots"><img src="/pics/katana_tux.jpg" width="180" alt="[Katana robot arm]"/></a></span>
There is a real opportunity for a robotic company to make a difference by
simply providing documentation and free software along with their product.
[Neuronics][1] in Switzerland is maybe the
only company currently targeting the industrial market with an industrial
precision (0.1 mm) robotic arm which comes with free software and documentation.
The product is called [Katana][2]. The Katana robot arm can carry 400 grams and it can be [controlled using TCP/IP (i.e. XML-RPC)][3] and the client library. The
robot's API supports point-to-point operation, customisation of speed and
acceleration, as well as spline-motions in configuration space. There also is
a native API for programming the control board directly and finally there is
the source code of the control software itself.

Neuronics is located in Zürich and there is potential for a collaboration
with local universities.

**See also:**

* [Katana specification][4]
* [Katana API documentation][5]
* [Katana robot at Linuxdevices.com][6]
* [Open Robot Control Software][7]
* [Willow Garage Personal Robot 2][8]
* [MeArm][9]

[1]: http://www.neuronics.ch/
[2]: http://www.neuronics.ch/cms_de/web/index.php?id=364&s=linux_robot
[3]: http://www.neuronics.ch/cms_de/web/index.php?id=374&s=code_snippets
[4]: http://www.neuronics.ch/cms_en/web/index.php?lid=54
[5]: http://www.neuronics.ch/cms_de/web/index.php?id=373&s=api_documentation
[6]: http://www.linuxdevices.com/news/NS7876661673.html
[7]: http://www.orocos.org/
[8]: http://www.willowgarage.com/pages/robots/pr2-overview
[9]: http://mearm.com/
