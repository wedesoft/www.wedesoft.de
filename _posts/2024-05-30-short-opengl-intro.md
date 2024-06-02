---
layout: post
title: A Short Introduction to OpenGL
category: software
image: /pics/opengl-intro.jpg
---

Here is a short introduction to OpenGL.

{% youtube "https://www.youtube.com/watch?v=dQQgHCK_lA8" %}

Click above image to watch the 30 minutes presentation.

You can get the slides here: [opengl.pdf][1]

See [github.com/wedesoft/opengl][2] for source code of slides and opengl-example.c.

Here are instructions on how to compile and run the program with GCC under GNU/Linux:
{% highlight shell %}
sudo apt install build-essential libglew-dev libglfw3-dev
wget https://www.wedesoft.de/downloads/opengl-example.c
gcc -o opengl-example opengl-example.c -lglfw -lGLEW -lGL -lm
./opengl-example
{% endhighlight %}

[1]: /downloads/opengl.pdf
[2]: https://github.com/wedesoft/opengl
