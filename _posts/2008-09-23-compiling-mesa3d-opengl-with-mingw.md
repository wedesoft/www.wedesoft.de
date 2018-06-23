---
layout: post
title: Mesa3D (OpenGL) and MinGW
image: /pics/gears.png
---

<span class="right"><img src="/pics/gears.png" width="146" alt=""/></span>

I've just managed to compile and install [Mesa3D][1] version 7.2 under Microsoft Windows (using MinGW). Here are the steps in case anyone else needs to know:

* [Install MinGW and MSYS][2]
* Download and unpack MesaLib, MesaGLUT, and MesaDemos into the same directory. The files are available on [SourceForge][3].
* Open the file _Mesa-7.2/Makefile.mgw_ in an editor and replace <br/><pre>copy progs\samples\star.exe lib</pre> with <br/><pre>cp progs/samples/star.exe lib</pre>
* Compile Mesa3D by executing <b>make -f Makefile.mgw</b> under MSYS
* Copy the files _Mesa-7.2/lib/lib&#42;.a_ to the directory _/mingw/lib_
* Copy the files _Mesa-7.2/lib/&#42;.dll_ to the directory _/mingw/bin_
* Copy the content of _Mesa-7.2/include_ to the directory _/mingw/include_

To test Mesa3D you can for example download SGI's [cube.c][4] and compile it by executing

{% highlight shell %}
gcc -o cube cube.c -lopengl32 -lglu32 -lglut32
{% endhighlight %}

[1]: http://www.mesa3d.org/
[2]: http://www.mingw.org/wiki/HOWTO_Install_the_MinGW_GCC_Compiler_Suite
[3]: http://sourceforge.net/project/showfiles.php?group_id=3
[4]: http://www.sgi.com/products/software/opengl/examples/glut/examples/source/cube.c
