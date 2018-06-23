---
layout: post
title: Racket on Android
image: /pics/racket.png
---

**A smart smartphone**
<span class="right"><a href="http://racket-lang.org/"><img src="/pics/racket.png" width="88" alt=""/></a></span>

The recent smartphones have quite impressive specs. *E.g.* the [Samsung Galaxy S III][1] comes with a 1.5 GHz quad-core processor and a GPU. It would be interesting though to run a *real* programming language on one of those phones. I am quite interested in the [Racket programming language][3] (also see [Wikipedia][4]) which offers full meta-programming and compile-time macros. I managed to cross-compile *Racket* to run on an Android phone [HTC Desire S][8]. I'll describe the steps here. Hopefully I'll find time to improve things and package it in some way.

**Build**

To cross-compile Racket for ARM you first need to compile *Racket* for the host system. The *GCC cross-compiler* and the *Racket interpreter* are then used during the cross-compilation.

{% highlight shell %}
#!/bin/sh
ARCHIVE=$HOME/Documents/programming/racket-5.2.1-src-unix.tgz
rm -Rf /tmp/build
mkdir -p /tmp/build/host
cd /tmp/build/host
tar xzf $ARCHIVE
BUILD_HOST=/tmp/build/host/racket*
cd $BUILD_HOST/src
./configure
make install
mkdir -p /tmp/build/cross
cd /tmp/build/cross
tar xzf $ARCHIVE
BUILD_CROSS=/tmp/build/cross/racket*
cd $BUILD_CROSS/src
./configure --host=arm-linux-gnueabi
make \
  RUN_THIS_RACKET_CGC=$BUILD_HOST/src/racket/racketcgc \
  RUN_THIS_RACKET_MMM=$BUILD_HOST/src/racket/racket3m \
  RUN_THIS_RACKET_MAIN_VARIANT=$BUILD_HOST/src/racket/racket3m \
  HOSTCC=/usr/bin/gcc \
  HOSTCFLAGS="-g -O2 -Wall -pthread -I./include" \
  STRIP_DEBUG="arm-linux-gnueabi-strip -S" 
make \
  RUN_THIS_RACKET_CGC=$BUILD_HOST/src/racket/racketcgc \
  RUN_THIS_RACKET_MMM=$BUILD_HOST/src/racket/racket3m \
  RUN_THIS_RACKET_MAIN_VARIANT=$BUILD_HOST/src/racket/racket3m \
  HOSTCC=/usr/bin/gcc \
  HOSTCFLAGS="-g -O2 -Wall -pthread -I./include" \
  STRIP_DEBUG="arm-linux-gnueabi-strip -S" \
  install
cd $BUILD_CROSS/src/racket
arm-linux-gnueabi-gcc -static -o racket3m  gc2/main.o libracket3m.a  -ldl -lm  -ldl -lm -rdynamic
{% endhighlight %}

The last command is used to replace the **racket3m** binary with a statically linked version.

**Installation**

First I installed [Android Terminal Emulator][5]. I also installed Kevin Boone's [kbox_shell][6] which allows you to run [BusyBox][7] without requiring to jailbreak the phone (I don't think *BusyBox* is required to run *Racket* but it is nice to have the various Linux shell utilities). I really recommend to install [Hacker's Keyboard][9] to have a full PC keyboard on your phone.

<span class="center"><a href="/pics/androidterm.jpg"><img src="/pics/androidterm.jpg" width="240" alt="Android Terminal Emulator"/></a></span>

Then one needs to copy the following files to the mobile phone:

* The **racket3m** binary is copied to the mobile in a location where it can be accessed from the terminal (*e.g.* */data/data/jackpal.androidterm/shared_prefs/kbox/bin/racket* if you have *kbox_shell* installed).
* Several *Racket* modules located in */tmp/build/cross/racket-5.2.1/collects/...* need to be copied to *$HOME/.racket/5.2.1/collects* on the mobile phone. The libraries are
  * *racket*
  * *syntax*
  * *setup*
  * *config*
  * *compiler*
  * *planet*
  * *mzlib*
  * *scheme*
  * *unstable*
  * *mzscheme*

**Racket on Android**
Unfortunately the full *Racket* language takes 90 seconds (!) to load on the *HTC Desire S*. Once loaded, the [REPL][10] is very responsive though.

<span class="center"><a href="/pics/androidracket.png"><img src="/pics/androidracket.png" width="400" alt="Racket interpreter running on Android"/></a></span>

Hopefully I get *ffi* and the *readline* library working to have an improved command line.

Any comments and suggestions are welcome <img src="/pics/bounce.gif" width="19" alt=""/>

**Update**

It is possible to only load the *Racket* base language. Running the following command

{% highlight shell %}
racket -I racket/base
{% endhighlight %}

brings up the REPL in 7 seconds. Thanks Robby for the helpful comment!

**Update**

Racket 5.3.0 furthermore needs the following module to be installed:

  * *s-exp* 

**Update**

I haven't managed to build Racket with the [Android NDK][11] (see [my thread on the Racket mailing list][12]). Building Racket with the Android NDK would be preferable because otherwise it is impossible to dynamically load Android libraries in order to access Android specific functionality.

[1]: http://www.amazon.co.uk/gp/product/B0080DJ6C2/ref=as_li_ss_tl?ie=UTF8&tag=wedesoft-21&linkCode=as2&camp=1634&creative=19450&creativeASIN=B0080DJ6C2
[2]: http://www.icg.tugraz.at/project/studierstube
[3]: http://racket-lang.org/
[4]: http://en.wikipedia.org/wiki/Racket_%28programming_language%29
[5]: https://play.google.com/store/apps/details?id=jackpal.androidterm
[6]: http://kevinboone.net/android_nonroot.html
[7]: http://www.busybox.net/
[8]: http://www.amazon.co.uk/gp/product/B004NNU9YM/ref=as_li_ss_tl?ie=UTF8&camp=1634&creative=19450&creativeASIN=B004NNU9YM&linkCode=as2&tag=wedesoft-21
[9]: https://play.google.com/store/apps/details?id=org.pocketworkstation.pckeyboard
[10]: http://en.wikipedia.org/wiki/Read-eval-print_loop
[11]: http://developer.android.com/tools/sdk/ndk/index.html
[12]: http://lists.racket-lang.org/dev/archive/2012-August/010311.html
