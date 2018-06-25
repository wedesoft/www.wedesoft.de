---
layout: post
title: RRobots - Robocode clone
category: robot
image: /pics/rrobots.png
---

I just stumbled upon [RRobots]. [RRobots] is a [C-Robots]/[Robocode] clone written entirely in [Ruby]. [RRobots] provides an arena in which robots - controlled by user code - compete against each other.

<span class="center"><img src="/pics/rrobots.png" width="400" alt=""/></span>

Since the programming language is Ruby, no compilation or linking is required. When you've finished coding the robot, you are ready to go. The software makes use of a set of images (turret, body, radar with different orientations, explosions). Amazingly the whole software including graphical visualisation, simulation, and mixin-code for the robots is only 835 lines of code. It uses Ruby-Tk for visualisation. Up to 8 robots can participate in a tournament.

{% highlight shell %}
ruby rrobots.rb bots/SniperDuck.rb bots/SporkBot.rb \
  bots/ShootingStation.rb bots/MsgBot.rb
{% endhighlight %}

You can download a [self-extracting archive with lots of robots][RobotPack] from the [robots forum].

[RRobots]: http://rubyforge.org/projects/rrobots/
[C-Robots]: http://en.wikipedia.org/wiki/Crobots">C-Robots
[Robocode]: http://robocode.sourceforge.net/">Robocode
[Ruby]: http://www.ruby-lang.org/
[RobotPack]: http://rubyforge.org/forum/forum.php?thread_id=4472&forum_id=4792
[robots forum]: http://rubyforge.org/forum/forum.php?forum_id=4792
