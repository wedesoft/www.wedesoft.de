---
layout: post
title: Run Europa Universalis IV with Epic Games on GNU/Linux
category: software
image: /pics/eu4.jpg
---

This is how I managed to run Europa Universalis IV via Epic Games on GNU/Linux (Debian 12).

First I [installed Wine 9.0 from the Wine package repositories][1].
Next I used [winetricks for add dependencies for the Epic Launcher][2].

```Shell
winetricks -q d3dcompiler_43 d3dcompiler_47 d3dx9 corefonts cjkfonts faudio
```

I then was able to install a working version of Epic Games.

```Shell
wine EpicInstaller-15.17.1.msi
```

Finally I installed Europa Universalis IV using Epic Launcher and run it.

![Europa Universalis IV on GNU/Linux (Debian 12)](/pics/eu4.jpg)

[1]: https://wine.htmlvalidator.com/install-wine-on-debian-12.html
[2]: https://forum.manjaro.org/t/howto-installing-epic-games-and-other-games-via-wine-without-lutris-and-proton/92236
