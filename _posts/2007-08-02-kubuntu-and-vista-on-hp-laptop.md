---
layout: post
title: Kubuntu and Vista on HP laptop
image: /pics/nx7400.jpg
---

Ignore this if you are not setting up a HP laptop right now. This is only for anyone who is looking for helpful information about installing **Kubuntu** on a **HP Compaq nx7400 notebook** with **Windows Vista Business** (dual boot). It may be similar on other laptops and with other versions of *Windows Vista* though.

![HP Compaq nx7400 notebook](/pics/nx7400.jpg)

## Backup and installation

* The laptop has a 80 GByte hard disk. Initially there is a 20 GByte partition with *Windows Vista Business* installation software on it
* On booting up the first time, the *Windows Vista* software is personalised and installed on the hard-disk.
* After *Windows Vista* is installed, HP's backup software automatically creates a backup on the hard-disk and you end up with three partitions: the windows partition, a recovery partition, and another partition with the recovery boot-loader.
* To be safe you can boot *Kubuntu* from DVD and create a backup of the master boot record, the partition table, and the first partition if you have a large external hard disk for example (alternatively maybe you can use the HP backup tool to create a backup on DVDs, but I am not sure about this):

{% highlight shell %}
dd if=/dev/sda bs=512 count=1 of=/media/disk/sdavista.mbr
fdisk -l /dev/sda > /media/disk/fdiskvista.log
dd if=/dev/sda1 of=/media/disk/sda1.img
# This will take some time
{% endhighlight %}

* In the "Control Panel" under *Windows Vista*, you can find the partition manager under the advanced tools. I deleted the second and third partition (the backup). This doesn't harm *Windows Vista* itself. Furthermore I shrank the system partition using "Shrink partition" to the allowed minimum size of 40 GByte.
* Now it was possible to install *Kubuntu*. The *Kubuntu* installation even allows you to further reduce the size of the *Windows Vista* partition (e.g. 25 GByte). Don’t forget to install a daemon for CPU frequency scaling such as *cpudyn* or *powersave*.
* Using *dd* (see above) one can now create a new backup of the reduced first partition (sda1.img):

{% highlight shell %}
dd if=/dev/sda bs=512 count=1 of=/media/disk/sdalinux.mbr
fdisk -l /dev/sda > /media/disk/fdisklinux.log
dd if=/dev/sda1 of=/media/disk/sda1.img
# This will take some time
{% endhighlight %}

* It can be useful to store the backup files on the Linux partition as well. One can use *gzip* to compress the img-file. To save disk-space you can compress the backup image on the fly:

{% highlight shell %}
dd if=/dev/sda1 | gzip -c > /usr/local/share/sda1.img.gz
# This will take quite some time
{% endhighlight %}

<h2>Restoring from backup</h2>

_If Windows Vista is corrupted due to some software problem_, now one does not need to have the external hard drive for restoring Windows Vista at hand. You would restore the content of the Windows Vista partition like this (_I did not try this!_):

{% highlight shell %}
gunzip -c /usr/local/share/sda1.img.gz | dd of=/dev/sda1
# This will take some time
{% endhighlight %}

One can use the information in _fdisklinux.log_ using the command *fdisk /dev/sda* to manually restore a partition layout. The mbr-files can be used to restore a particular master-boot record. The img-file can be used to restore the 25GByte *Windows Vista* partition.

All this information is given without any warranty. You can send me an e-mail if you find that the information is wrong.

Note that if you uninstall the HP backup tool, it will overwrite the master-boot-record of the hard disk (without confirmation) and you will have to reinstall grub (see reference below).

**Update:**

*Dabs* is now known as [BT Shop](https://www.shop.bt.com/).

**See also:**

* [Linux on Hp Compaq nx7400][nx7400]
* [Linux on HP Laptops &amp; Notebooks][hplinux]
* [How to restore GRUB bootloader with an Ubuntu Live-CD][restoregrub]
* [Getting Wireless iwl3945 to work on Hardy][backports]
* [Using WICD instead of Network Manager][wicd]

[nx7400]: http://emisca.altervista.org/nx7400/
[hplinux]: http://www.linux-on-laptops.com/hp.html
[restoregrub]:http://ubuntuforums.org/showthread.php?t=224351
[backports]:http://ubuntuforums.org/showpost.php?p=4799471&postcount=29
[wicd]:http://wicd.net/
