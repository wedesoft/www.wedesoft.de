---
layout: post
title: Minimal OpenGL example
category: software
---

OpenGL is a powerful cross-platform standard for 3D visualisation.
OpenGL libraries use a domain specific language (shader language) to describe element-wise operations on vertices (vertex shader) and pixel values (fragment shader).
More recent OpenGL versions also support geometry shaders and tesselation shaders (see [OpenGL article on Wikipedia](https://de.wikipedia.org/wiki/OpenGL_Shading_Language)).

The learning curve for OpenGL is quite steep at the beginning.
The reason is, that a program to draw a triangle is almost as complex as a program drawing thousands of triangles.
It is also important to add code for retrieving error messages in order to be able to do development.

I haven't found many minimal examples to understand OpenGL, so I am posting one here.
The example draws a coloured triangle on the screen.

{% gist f16a921835194aa57d623a9be4014f84 raw-opengl.c %}

The example uses the widely supported OpenGL version 3.1 (which has the version tag 130).
You can download, compile, and run the example as follows:

{% highlight shell %}
wget https://gist.githubusercontent.com/wedesoft/f16a921835194aa57d623a9be4014f84/raw/485dff31777c25a5ef0b08c48943df7ec7ca7624/raw-opengl.c
gcc -o raw-opengl raw-opengl.c -lGL -lGLEW -lGLU -lglut
./raw-opengl
{% endhighlight %}

![image](/pics/triangle.png)

Any feedback, comments, and suggestions are welcome.

Enjoy!
