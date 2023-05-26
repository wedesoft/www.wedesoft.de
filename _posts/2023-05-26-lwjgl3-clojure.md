---
layout: post
title: OpenGL example in Clojure using LWJGL version 3
category: software
image: /pics/triangle.png
---

Some time ago I published a [minimal OpenGL example in Clojure using LWJGL version 2][6].
I used LWJGL 2 because it is available as a Debian package unlike version 3.
However one can get [LWJGL 3.3.2][2] with native bindings from the [Maven Central Repository][1].

Note that LWJGL version 3 consists of [several packages][7].
Furthermore most packages have native bindings for different platforms.
See [LWJGL customization page][4] for more information.
There is a [Leiningen LWJGL project example][3] by Roger Allen which shows how to install all LWJGL libraries using Leiningen.
However here we are going to use the more modern approach with a *deps.edn* file and we are going to use more efficient OpenGL indirect rendering.

The *deps.edn* file [uses symbols of the form \<groupID\>/\<artifactId\>$\<classifier\>][5] to specify libraries.
For LWJGL version 3 the classifier is used to install native extensions.
The *deps.edn* file for installing LWJGL with OpenGL and GLFW under GNU/Linux is as follows:

{% gist cd60b92bb34c58a61ccf2ff1544c6bf3 deps.edn %}

There are more [native packages for lwjgl-opengl][8] such as *natives-windows* and *natives-macos*.

Displaying a window with GLFW instead of LWJGL 2 is a bit different.
The updated code to use lwjgl-glfw and lwjgl-opengl to display a triangle with a small texture is shown below:

{% gist cd60b92bb34c58a61ccf2ff1544c6bf3 raw-opengl-lwjgl3.clj %}

If you download the two files, you can run the code as follows:

{% highlight shell %}
clj -M raw-opengl-lwjgl3.clj
{% endhighlight %}

It should show a triangle like this:

![image](/pics/triangle.png)

Any feedback, comments, and suggestions are welcome.

Enjoy!

[1]: https://central.sonatype.com/
[2]: https://central.sonatype.com/artifact/org.lwjgl/lwjgl/3.3.2
[3]: https://github.com/rogerallen/hello_lwjgl
[4]: https://www.lwjgl.org/customize
[5]: https://clojure.org/reference/deps_and_cli#_dependencies
[6]: https://www.wedesoft.de/software/2020/12/10/opengl-clojure/
[7]: https://central.sonatype.com/search?smo=true&namespace=org.lwjgl&sort=name
[8]: https://repo1.maven.org/maven2/org/lwjgl/lwjgl-opengl/3.3.2/
