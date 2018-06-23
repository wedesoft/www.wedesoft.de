---
layout: post
title: Debian Stretch
image: /pics/debian9.png
---

**[Debian][debian]** is a software libre operating system for personal computers.
The new *[Debian Stretch (9)][stretch]* distribution of *GNU/Linux* is the first *Debian* release to include the new [KDE Plasma 5][plasma] desktop.

<span class="center"><a href="pics/debian-kde-plasma.png"><img src="pics/debian-kde-plasma.png" width="508" alt="Debian Stretch (9) with KDE Plasma 5.8"/></a></span>

* Download the [ISO image][iso] from the [non-free section][nonfree] of the Debian file server.
  The *non-free* installation medium contains proprietary firmware and drivers for WiFi adapters and graphics cards where the manufacturer has not released a free software driver.
* Write the big *ISO* file to a USB stick using a tool for creating bootable USB sticks and boot from it.
  You might have to enable booting from USB in the BIOS settings before booting.
* Install Debian with KDE Plasma as desktop and reboot. Make sure that software updates are working.
* Install required proprietary firmware and drivers to get WiFi, Bluetooth, and 3D graphics working.
  In my case I had to install firmware for the *Qualcomm Atheros QCA9565 / AR9565 Wireless Network Adapter* and drivers for the *AMD/ATI Kabini Radeon HD 8330* graphics card.
  Note that I currently have unsolved connectivity and low signal issues with the Qualcomm Atheros WiFi adapter.
  Instead I am accessing WiFi using USB tethering with an Android phone.

{% highlight shell %}
sudo apt-get install firmware-atheros firmware-amd-graphics
sudo apt-get install firmware-linux-nonfree libgl1-mesa-dri xserver-xorg-video-ati
{% endhighlight %}

* Add [Debian Multimedia][media] to get patent-encumbered functionality to work:

{% highlight shell %}
echo deb http://www.deb-multimedia.org stretch main non-free | sudo tee /etc/apt/sources.list.d/deb-multimedia.list
wget http://www.deb-multimedia.org/pool/main/d/deb-multimedia-keyring/deb-multimedia-keyring_2016.8.1_all.deb
sudo dpkg -i deb-multimedia-keyring_2016.8.1_all.deb
sudo apt-get update
sudo apt-get dist-upgrade
{% endhighlight %}

* Install *gstreamer1.0-libav* to get the [JuK][juk] music player to work:

{% highlight shell %}
sudo apt-get install gstreamer1.0-libav
{% endhighlight %}

* Install *qml-module-qtquick-xmllistmodel* to get the network traffic display to work:

{% highlight shell %}
sudo apt-get install qml-module-qtquick-xmllistmodel
{% endhighlight %}

* Setup [Debian support for Bluetooth headsets][a2dp]. This requires proprietary Bluetooth firmware:

{% highlight shell %}
sudo apt-get install pulseaudio pulseaudio-module-bluetooth pavucontrol bluez-firmware
{% endhighlight %}

* After rebooting you can pair a Bluetooth headset and use *pavucontrol* to redirect the output of a running media player to it.

More software I recommend:

* [Clementine music player][clementine].
* [Electrum instant on Bitcoin wallet](https://electrum.org/)
* [LaTeX text processing](https://www.latex-project.org/)
* [Plymouth boot splash screen][plymouth]
* [Deja-Dup backup solution][dejadup] ([needs python-gi](https://answers.launchpad.net/deja-dup/+question/276745))
* [KMyMoney personal finance manager](https://kmymoney.org/)
* [Hugin panorama photo stitcher](http://hugin.sourceforge.net/)
* [Luminance HDR](http://qtpfsgui.sourceforge.net/)

I'll try to keep this up to date as I discover more useful things to install and setup.

Enjoy!

**See also:**

* [Debian ATI Howto](https://wiki.debian.org/AtiHowTo)
* [Debian Bluetooth a2dp][a2dp]
* [KDE Wallet migration repeated at each login][wallet].
  Need to set *~/.config/kwalletrc* to the following content for it to go away.

{% highlight ini %}
[Migration]
alreadyMigrated=true

[Wallet]
First Use=false
{% endhighlight %}

* Had to reenable Baloo file indexing using *balooctl enable*

[debian]: https://www.debian.org/
[stretch]: https://wiki.debian.org/DebianStretch
[plasma]: https://www.kde.org/screenshots
[nonfree]: https://cdimage.debian.org/cdimage/unofficial/non-free/cd-including-firmware/9.0.0+nonfree/amd64/iso-dvd/
[iso]: https://cdimage.debian.org/cdimage/unofficial/non-free/cd-including-firmware/9.0.0+nonfree/amd64/iso-dvd/firmware-9.0.0-amd64-DVD-1.iso
[media]: http://www.deb-multimedia.org/
[juk]: https://www.kde.org/applications/multimedia/juk/
[jukfix]: https://forum.kde.org/viewtopic.php?f=19&t=131924
[clementine]: https://www.clementine-player.org/
[a2dp]: https://wiki.debian.org/BluetoothUser/a2dp
[wallet]: https://forums.opensuse.org/showthread.php/510917-New-leap-final-installation-on-laptop-On-each-login-get-twice-Kwalletmigration-agent-asking-for-pa
[plymouth]: https://wiki.debian.org/plymouth
[dejadup]: https://launchpad.net/deja-dup
