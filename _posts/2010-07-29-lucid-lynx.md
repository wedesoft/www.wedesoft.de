---
layout: post
title: Kubuntu 10.04
image: /pics/lucid.jpg
---

I finally decided to update my laptop to **[Kubuntu 10.04][Kubuntu]** (a distribution of GNU/Linux based on [Debian]). As usual I used the Live DVD to just rename the _/home_ folder to _/old_, delete everything else, and mount the partitions in the advanced setup without formatting them (although like this I wasn't able to make use of the new EXT4 file system). For new users there is an article how to do a [graphical installation of Kubuntu][install].

KDE4 really has some nice eye candy. Also there are themes for many things and you can just download them using the configuration dialogs. Here's a screenshot of the [Kubuntu Spotlight] theme I chose for the login manager.
<span class="center"><a href="http://kde-look.org/content/show.php/Kubuntu+Spotlight+KDM?content=114234"><img src="/pics/kdm.jpg" width="508" alt="Kubuntu Spotlight KDM"/></a></span>

For the boot splash animation I used the [Spectrum] theme.
<span class="center"><a href="http://kde-look.org/content/show.php/Spectrum?content=111069"><img src="/pics/spectrum.jpg" width="508" alt="Spectrum Splash Screen"/></a></span>

And here is a screenshot of the desktop in action.
<span class="center"><a href="http://www.kubuntu.org/"><img src="/pics/lucid.jpg" width="508" alt="Kubuntu 10.04 Lucid Lynx (LTS)"/></a></span>

The distribution can be downloaded free of charge at [http://www.kubuntu.org/][Kubuntu]. There are few issues with the [HP Compaq nx7400 notebook][Compaq] I am using. So far I've found the following problems:

* [NetworkManager disables itself][Network] before going into standby and doesn't come back when disconnecting power during standby mode (one needs to edit the file */var/lib/NetworkManager/NetworkManager.state*). Also when using another tool such as pppoeconfig, [NetworkManager will disable itself using another configuration file][Network2] (one needs to edit the file */etc/NetworkManager/nm-system-settings.conf*).
* VoIP settings: One needs to switch everything in Twinkle to ALSA/Default.
* When not terminated properly, Amarok gives the error message "Amarok is already running!" even though it is not running.
* Sometimes one needs to restart Amarok because it stops playing after every song.
* The Shoutcast list is not included in Amarok any more but there is an Amarok script for [adding Shoutcast to Amarok][Shoutcast].
* I had to install [msn-pecan] because Pidgin's default implementation for MSN has issues.
* Pidgin won't connect to ICQ unless _Use clientLogin_ is checked in the advanced account settings.
* The waste bin has become a plasma widget which is not on the desktop by default.
* As usual one needs to fetch additional codecs and libdvdcss from the [Medibuntu] repository in order to be able to play videos using patent-encumbered codecs and DVDs.
* The file associations for PDF documents are configured to prefer the proprietary Acrobat Reader over the Okular free software.
* WPA2 wireless keeps disconnecting (iwl3945 driver, deauthenticating by local choice (reason=3)). Installing [install the linux backports modules][backports] doesn't seem to help either at this point in time.
* The default output device for mplayer is _xmga_. You need to change it to _xv_.
* One of the ALSA volume controls for the microphone seems to be mislabelled as the 'Digital' input.

**Update:**

I posted about [listing manually selected packages with aptitude][apt] on the Ubuntu Forum. This is very useful for generating the command line for installing the same set of packages on a new machine (*e.g.* my new [HP G56-108SA][g56] ;)). One can list the manually selected packages as follows:

{% highlight shell %}
aptitude search '!~M ~i' -F '%p'
{% endhighlight %}

*I.e.* you can run the following command on one machine:

{% highlight shell %}
echo aptitude install `aptitude search '!~M ~i' -F '%p'` > install.sh
{% endhighlight %}

Then you copy the resulting file *install.sh* to the target machine and run

{% highlight shell %}
sh ./install.sh
{% endhighlight %}

Basically **!~M** selects not automatically selected packages and **~i** selects installed packages. See content of package **aptitude-doc-en** for more information.

Note that you might have to manually modify the file *install.sh* if the package names on the target system are different (*e.g.* 32-bit vs. 64-bit, other proprietary wireless driver, ...).

**Update:**

Here's a nice example of a dual-screen desktop running the same *Kubuntu* version.

{% youtube "http://www.youtube.com/watch?v=s8gWP_CGTpA" %}

**See also:**

* [Kubuntu]
* [Debian]
* [KDE]
* [Mark Shuttleworth keynote at Open World Forum][owf]
* [Mark Shuttleworth interview at ONLamp][oreilly]
* [Setting up a DNS cache with dnsmasq][dnsmasq]
* [Day and Night Wallpaper Plasmoid Script][dan]
* [Hybrid:Works][hybrid] (especially see [Desktop Vacation 2008][dv])
* [Ghost in the Shell theme for KDE4][gits]
* [Smooth Tasks for KDE][smooth]
* [Dropbox and KDE][dropbox]
* [Choqok 1.1 update by Adilson dos Santos Dantas][choqok]

[Kubuntu Spotlight]: http://kde-look.org/content/show.php/Kubuntu+Spotlight+KDM?content=114234
[Spectrum]: http://kde-look.org/content/show.php/Spectrum?content=111069
[Kubuntu]: http://www.kubuntu.org/
[Network]: http://ubuntuforums.org/showthread.php?t=1475230
[Network2]: https://bugs.launchpad.net/ubuntu/+source/plasma-widget-networkmanagement/+bug/465591
[Compaq]: http://www.wedesoft.de/kubuntu-and-vista-on-hp-laptop.html
[msn-pecan]: http://code.google.com/p/msn-pecan/
[trash]: http://osdir.com/ml/kubuntu-users/2010-05/msg00204.html
[Debian]: http://www.debian.org/
[KDE]: http://kde.org/
[Medibuntu]: http://medibuntu.org/
[owf]: http://www.youtube.com/watch?v=FPxTdr4S5WE
[oreilly]: http://onlamp.com/pub/a/onlamp/2008/04/15/an-interview-with-mark-shuttleworth.html
[install]: https://help.ubuntu.com/community/GraphicalInstall/Kubuntu
[Shoutcast]: http://kde-apps.org/content/show.php/SHOUTcast+service?content=116823
[dnsmasq]: http://wiki.debian.org/HowTo/dnsmasq
[backports]: http://linuxwireless.org/en/users/Download#Getting_compat-wireless_on_Ubuntu
[gits]: http://kde-look.org/content/show.php/Ghost+in+the+Shell?content=127580
[dan]: http://kde-look.org/content/show.php/Day+and+Night+Wallpaper?content=118636
[hybrid]: http://www.hybridworks.jp/
[dv]: http://www.hybridworks.jp/readme/desktopvacation2008/
[apt]: http://ubuntuforums.org/showthread.php?t=1364639#6
[smooth]: http://www.youtube.com/watch?v=wqcVxN7JiwI
[g56]: http://www.dealgiant.co.uk/hp-g56-108sa-laptop-review/
[dropbox]: http://www.nixternal.com/kde-and-dropbox/
[choqok]: https://launchpad.net/~adilson/+archive/experimental

