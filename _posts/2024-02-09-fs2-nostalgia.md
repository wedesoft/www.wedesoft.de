---
layout: post
title: Flight Simulator 2 on Atari ST
category: software
image: /pics/fs2.png
---

As a kid I got the joy of playing Flight Simulator 2 on my dad's Atari 1040ST.
The Atari had a 640x400 monochrome monitor.

I noticed that Debian 12 comes with the [Hatari][1] Atari emulator.
Furthermore one can download a disk image of [Flight Simulator 2][2] at atarimania.com.
Then it was just a matter of unzipping the file and specifying the disk image on the Hatari command line.

{% highlight shell %}
sudo apt install hatari
unzip flight_simulator_2.zip
hatari --mono --disk-a "Flight Simulator 2.stx"
{% endhighlight %}

I took a video of the initial part of the demo mode here:

{% youtube "https://www.youtube.com/watch?v=nCGCYvRd0FA" %}

Enjoy!

Update: You also need to [download a TOS image for Atari ST][3] and copy it to `/usr/share/hatari/tos.img`.

[1]: https://hatari.tuxfamily.org/
[2]: http://www.atarimania.com/game-atari-st-flight-simulator-ii_9342.html
[3]: http://www.avtandil.narod.ru/tose.html
