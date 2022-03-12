---
layout: post
title: Debian 11 on the Tuxedo Aura 15 Gen1 laptop
category: technology
image: /pics/tuxedo-aura-15-gen1.jpg
---

## Buying a laptop from Tuxedo Computers

I upgraded to a laptop from [Tuxedo Computers][1] ([german page][2]).
The nice thing about Tuxedo Computers is that

* they provide GNU/Linux support for new hardware
* they let you customise the laptop (e.g. CPU, SSD size, RAM configuration, keyboard layout)
* the laptops are repairable/upgradeable
* the laptops come with 2 years warranty

Also note that [Tuxedo Computers are now a KDE patron][15].

I chose the [Tuxedo Aura 15 Gen1][3] ([german page][4]) with an [AMD Ryzen 7 4700U][12] CPU and an [AMD Radeon RX Vega 7][13] GPU.
I selected the laptop because of

* a keyboard with a number pad and backlight
* 15.6 inch display
* 8 core, 8 threads AMD CPU using only 15W power
* no proprietary NVidia graphics drivers

It is recommended when choosing the RAM to use a [multi-channel configuration][16] for better performance.
Also make sure to order enough RAM because some of it is used by the integrated graphics card.
Finally doing parallelized builds on many cores requires more memory.

Note that there also is the [Tuxedo Pulse 15 Gen1][24] which comes with a 8 core 16 threads AMD Ryzen 7 4800H.
Also the [Tuxedo Aura 15 Gen2][32] is due to arrive in April and also comes with 16 threads.

[Here][19] is an in-depth review in German language (and [here][17] is a shorter review).
Also there is a Youtube video review in English below:

{% youtube "https://www.youtube.com/watch?v=vjVXzuPIsS0" %}

## Installing Debian 11

On Reddit I got [helpful information][5] from Ferdinand from Tuxedo Computers on how to install Debian 11 on the Aura 15 Gen1.
Also see [Debian Wiki][6] for some information.
Debian is [not supported][23] by Tuxedo Computers but it works nonetheless on a Tuxedo Aura 15 Gen1.
I followed the following steps (no warranty) to get Debian 11 running:

* **Download** the [latest Debian 11 unofficial image including non-free firmware][20] and **create a bootable USB stick** using [Rufus][21] for example. The unofficial image comes with proprietary firmware for the AMD GPU and for the Wifi adapter.
* Boot with the USB stick plugged in, press **F7** and select the USB stick.
* You need to plug in the **network cable** for installing the updates. The Wifi doesn't work during installation.
* The Debian installer lets you choose which desktop (I chose **KDE** here).
* After installation you need to **comment out the cdrom entries** in /etc/apt/sources.list.

<span class="center"><img src="/pics/neofetch2.png" width="508" alt=""/></span>

## Installing Tuxedo packages and other packages

* You then need to add the Tuxedo package repository (see [here][22] for more information):

{% highlight shell %}
sudo nano /etc/apt/sources.list.d/tuxedocomputers.list
# Enter "deb https://deb.tuxedocomputers.com/ubuntu focal main" and save.
wget -O - https://deb.tuxedocomputers.com/0x54840598.pub.asc | sudo apt-key add -
sudo apt-get update
{% endhighlight %}

* Install *firmware-linux*.
* Manually install old [libappindicator3-1 and libindicator3-7 package][25] from Debian 10.
* You can then install the following three Tuxedo packages:
  * tuxedo-control-center (user interface for power management) ([source][10])
  * tuxedo-keyboard (dkms kernel module for keyboard lighting) ([source][11])
  * tuxedo-restore-audio-fix (options snd-hda-intel probe_mask=1) (see [package repository][7])
* Install packages from [deb-multimedia.org][26].
* [Install Plymouth boot splash][27].

<span class="center"><img src="/pics/tuxedo-debian.jpg" width="508" alt=""/></span>

Then a few more tweaks:

* I installed *plasma-browser-integration* and the corresponding browser addon (available for Firefox and Chrome/Chromium)
* I installed *ffmpegthumbs* and *kdegraphics-thumbnailers* and then enabled the thumbnail generators in the Dolphin settings
* I installed *bleachbit* and run it to clean up unused disk space

Finally you can list the installed packages on an old system to install them on the new machine.
*I.e.* you can run the following command on the old machine:

{% highlight shell %}
echo aptitude install `aptitude search '!~M ~i' -F '%p'` > install.sh
{% endhighlight %}

Then you copy the resulting file *install.sh* to the target machine and run

{% highlight shell %}
sh ./install.sh
{% endhighlight %}

{% youtube "https://www.youtube.com/watch?v=faUdi3-mvgI" %}

Enjoy!

### Updates

* Had to [edit 40-libinput.conf][28] to get touchpad tapping to work
* Tuxedocomputers also gives you 10GByte of space on their Nextcloud server which you can [mount using davfs2][29]
* Press **F2** to enter the BIOS; press **F7** to select the boot device
* If you want to set up a Windows/Linux dual-boot (using Grub) you need to disable Bitlocker encryption
* If you want to mount an NTFS partition you need to disable fast boot on Windows
* The BIOS supports [Flexicharger][30] which lets you set charge limits to extend battery life
* You can also set the keyboard backlight color using [webketje/tuxedo-backlight-control][31]

[1]: https://www.tuxedocomputers.com/en
[2]: https://www.tuxedocomputers.com/de
[3]: https://www.tuxedocomputers.com/en/Linux-Hardware/Linux-Notebooks/15-16-inch/TUXEDO-Aura-15-Gen1.tuxedo
[4]: https://www.tuxedocomputers.com/de/Linux-Hardware/Linux-Notebooks/15-16-Zoll/TUXEDO-Aura-15-Gen1.tuxedo
[5]: https://www.reddit.com/r/tuxedocomputers/comments/s4vsdx/debian_11_on_tuxedo_hardware/
[6]: https://wiki.debian.org/InstallingDebianOn/TUXEDO/NOTEBOOKS/TUXEDO-Aura-15-Gen1
[7]: https://deb.tuxedocomputers.com/ubuntu/pool/main/t/
[8]: https://www.tuxedocomputers.com/en/Infos/Help-and-Support/Instructions/Add-TUXEDO-Computers-software-package-sources.tuxedo
[9]: https://www.tuxedocomputers.com/en/Notes-on-the-use-of-operating-systems-that-are-not-officially-supported_1.tuxedo
[10]: https://github.com/tuxedocomputers/tuxedo-control-center
[11]: https://github.com/tuxedocomputers/tuxedo-keyboard
[12]: https://askgeek.io/en/cpus/AMD/Ryzen-7-4700U
[13]: https://askgeek.io/en/gpus/AMD/Radeon-RX-Vega-7
[14]: https://kde.org/
[15]: https://dot.kde.org/2021/09/14/tuxedo-computers-becomes-newest-kde-patron
[16]: https://en.wikipedia.org/wiki/Multi-channel_memory_architecture
[17]: https://linuxnews.de/2020/12/ein-erster-blick-auf-das-tuxedo-aura-15/
[18]: https://www.dummies.com/article/technology/computers/pcs/how-to-shrink-a-hard-drive-volume-in-windows-183270
[19]: https://www.notebookcheck.com/Tuxedo-Aura-15-Gen1-im-Test-Office-Laptop-mit-AMD-Ryzen-und-Linux.531830.0.html
[20]: https://cdimage.debian.org/cdimage/unofficial/non-free/cd-including-firmware/current/amd64/iso-dvd/
[21]: https://rufus.ie/
[22]: https://www.tuxedocomputers.com/en/Infos/Help-and-Support/Instructions/Add-TUXEDO-Computers-software-package-sources.tuxedo
[23]: https://www.tuxedocomputers.com/en/Notes-on-the-use-of-operating-systems-that-are-not-officially-supported_1.tuxedo
[24]: https://www.tuxedocomputers.com/en/Linux-Hardware/Linux-Notebooks/15-16-inch/TUXEDO-Book-Pulse-15-Gen1.tuxedo
[25]: https://stackoverflow.com/questions/65978703/missing-libappindicator3-1-installing-slack
[26]: https://deb-multimedia.org/
[27]: https://wiki.debian.org/plymouth
[28]: https://askubuntu.com/questions/1179275/enable-tap-to-click-kubuntu
[29]: https://docs.nextcloud.com/server/20/user_manual/en/files/access_webdav.html
[30]: https://www.tuxedocomputers.com/en/Infos/Help-Support/Frequently-asked-questions/What-is-Flexicharger-.tuxedo
[31]: https://github.com/webketje/tuxedo-backlight-control
[32]: https://www.tuxedocomputers.com/en/Linux-Hardware/Linux-Notebooks/15-16-inch/TUXEDO-Aura-15-Gen2.tuxedo
