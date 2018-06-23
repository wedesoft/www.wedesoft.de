---
layout: post
title: Android Hello World
image: /pics/android.png
---

Here a quick documentation on how to build a C program using the [Android NDK for Linux][1]. The program can be run with the [Android Terminal][2].

Here's the *Makefile*.

{% highlight make %}
NDK = $(HOME)/android-ndk-r8b
TOOLCHAIN = /tmp/ndk-hello
SYSROOT = $(TOOLCHAIN)/sysroot
GCC = $(TOOLCHAIN)/bin/arm-linux-androideabi-gcc
STRIP = $(TOOLCHAIN)/bin/arm-linux-androideabi-strip
CFLAGS = -march=armv7-a -mfloat-abi=softfp -I$(SYSROOT)/usr/include
LDFLAGS = -Wl,--fix-cortex-a8 -L$(SYSROOT)/usr/lib

all: $(TOOLCHAIN) hello

hello: hello.o
	$(GCC) $(LDFLAGS) -o $@ hello.o
	$(STRIP) -s $@

.c.o:
	$(GCC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f hello *.o .*.un~

$(TOOLCHAIN):
	$(NDK)/build/tools/make-standalone-toolchain.sh --install-dir=$@
{% endhighlight %}

And here's the C program *hello.c*.

{%highlight c %}
#include <stdio.h>

int main(int argc, char** argv) {
   printf("Hello world!!!\n");
   return 0;
}
{% endhighlight %}

Running *make* should cross-compile the program.

[1]: http://developer.android.com/tools/sdk/ndk/index.html
[2]: https://play.google.com/store/apps/details?id=jackpal.androidterm

