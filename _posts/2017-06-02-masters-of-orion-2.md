---
layout: post
title: Masters of Orion II
image: /pics/moo2.png
---

<span class="center"><a href="/pics/moo2.png" alt="Masters of Orion II"><img src="/pics/moo2.png" width="384" alt="Masters of Orion II"/></a></span>

<h2>Running Masters of Orion II with Wine</h2>

Just want to point out this great game which is available free of charge this days.

You can download [Master Of Orion II][moo] from gamesnostalgia.com.

And it runs unter *GNU/Linux* thanks to *Wine*:

{% highlight shell %}
7z x masteroforion2_dos_win.7z
cd MasterOfOrion2/mastori2
winetricks ddr=gdi
wine SETSOUND.EXE
wine ORION95.EXE
{% endhighlight %}

Enjoy!

**Update:**

*Wine 1.8.7* under *Debian Stretch* displayed a blank screen when starting the game.
Using [*winetricks ddr=gdi*][trick] as shown above seems to resolve that issue.

[moo]: http://gamesnostalgia.com/en/game/master-of-orion-ii-battle-at-antares
[trick]: https://forum.winehq.org/viewtopic.php?t=14411
