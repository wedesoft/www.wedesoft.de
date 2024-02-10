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

<span class="center"><video controls src="/downloads/fs2-mono.mp4" width="100%"></video></span>

Enjoy!

[1]: https://hatari.tuxfamily.org/
[2]: http://www.atarimania.com/game-atari-st-flight-simulator-ii_9342.html
