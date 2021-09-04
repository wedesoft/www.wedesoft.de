---
layout: post
title: Installing Debian 11 (bullseye) on a real machine
category: software
image: /pics/debian.png
---

I installed Debian 11 with KDE desktop on my HP 255 G3 notebook.

Because my computer has an AMD GPU it is recommended to download the [unofficial Debian image with firmware][1].
The unofficial image also helps with Wireless adapters which frequently require proprietary firmware.
I downloaded the image which is 4 GByte in size and wrote it to a USB stick:

{% highlight shell %}
sudo dd if=firmware-11.0.0-amd64-DVD-1.iso of=/dev/sdb
{% endhighlight %}

Replace `/dev/sdb` with your correct USB location! You can see the location in the log (run `sudo dmesg`) after plugging it in.
Under Windows you can use a tool such as [Rufus][4] to create a bootable USB stick.

I usually create a list of packages of my current system so that I can later install the same packages on the new system.

{% highlight shell %}
aptitude search '!~M ~i' -F '%p' > install.txt
{% endhighlight %}

I rebooted the computer.
My laptop uses UEFI secure boot so you need to press Escape and then F9 to get a list of bootable devices.
The Debian installer image on the USB stick should show up in the list. Select it and press return.
I proceeded to use the graphical installer.

The Debian installer can shrink Windows partitions if you want to set up dual boot.
Note that you need to disable Windows fast startup, otherwise your Windows file system will be in a hibernated state.

On my machine I created a swap partition about the same size as the computer memory.
I already have a 16 GByte root (`/`) ext4 partition (should be a bit bigger ideally) and a large home (`/home`) ext4 partition.
I just format the root partition and simply mount the home partition without formatting.
After setting up the users I can already continue using the system.

<span class="center"><img src="/pics/neofetch.png" width="508" alt=""/></span>

I noticed that even the unofficial image is missing some firmware.
So I cloned the Linux firmware repository and copied the firmware files:

{% highlight shell %}
git clone --depth 1 https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git
cd linux-firmware
sudo make install
{% endhighlight %}

I then proceeded with the [instructions on how to set up Plymouth boot splash][2].

Furthermore I added [deb-multimedia.org][3] to the package sources and updated the multimedia software.

Finally a few more tweaks:
* I installed *plasma-browser-integration* and the corresponding browser addon (available for Firefox and Chrome/Chromium)
* I installed *ffmpegthumbs* and *kdegraphics-thumbnailers* and then enabled the thumbnail generators in the Dolphin settings
* I installed *bleachbit* and run it to clean up unused disk space

I hope this installation summary is useful to you!
Enjoy!

[1]: https://cdimage.debian.org/cdimage/unofficial/non-free/cd-including-firmware/current/amd64/iso-dvd/
[2]: https://wiki.debian.org/plymouth
[3]: https://deb-multimedia.org/
[4]: https://rufus.ie/
