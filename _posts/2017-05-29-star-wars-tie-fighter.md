---
layout: post
title: TIE Fighter flight simulator
image: /pics/tie.png
---

<span class="center"><a href="/pics/tie.png" alt="TIE Fighter"><img src="/pics/tie.png" width="384" alt="TIE Fighter"/></a></span>

<h2>Installing TIE Fighter on DOSBox</h2>

The [Internet Archive][archive] hosts among other vintage PC games a copy of [Star Wars TIE Fighter][cdrom].
I got the game to run under GNU/Linux using [DOSBox][dosbox]. I only got it to run using a Joystick.
The mouse controls didn't seem to work properly.

Download TIE Fighter from [here][cdrom] and place the files *SWTieCD/cd/SWTIECD.cue* and *SWTieCD/cd/SWTIECD.BIN* in a directory of your choice.
Also rename them to lower case (i.e. *swtiecd.cue* and *swtiecd.bin*).

Edit your DOSBox configuration file (in my case *$HOME/.dosbox/dosbox-0.74.conf* and [set the Joystick type to two axis][vogons] (it is set to *ch* by default).

{% highlight text %}
[joystick]
joysticktype=2axis
timed=false
swap34=true
buttonwrap=false
{% endhighlight %}

Change to the directory where the CD image is and run DOSBox as follows:
{% highlight text %}
dosbox .
{% endhighlight %}

In the DOSBox window mount the CD image as follows:

{% highlight text %}
imgmount d c:\swtiecd.cue -t iso
{% endhighlight %}

Now you can install and run the game:

{% highlight text %}
d:\install
{% endhighlight %}

Enjoy!

**See also:**

* [Star Wars TIE Fighter Collectors CD ROM][archive]
* [TIE Fighter Joystick problems][vogons]
* [Top 30 abandonware games by genre][best]

[archive]: https://archive.org/
[dosbox]: http://www.dosbox.com/
[img]: https://www.dosbox.com/wiki/IMGMOUNT
[cdrom]: https://archive.org/details/msdos_Star_Wars_TIE_Fighter_Collectors_CD-ROM_1995
[vogons]: http://www.vogons.org/viewtopic.php?t=15318
[best]: http://www.abandonwaredos.com/best-retro-games.php
