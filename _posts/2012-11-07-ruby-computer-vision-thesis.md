---
layout: page
title: Developing machine vision software with Ruby instead of C/C++
image: /pics/otsuout.png
---

When I started doing a PhD in machine vision in 2004 I didn't know what I was in for. I thought I would learn about various object recognition algorithms, implement them in C++, and then try to come up with something new. I was motivated to implement 2D object recognition and tracking algorithms and I was hoping to eventually get into 3D object recognition/tracking and/or [Visual SLAM][1] (simultaneous localisation and mapping).

The trouble started when I started to realise how many representations of images there are. I am not even talking about [colour spaces][2] or compressed images/videos. It is already sufficient to just consider one-channel grey images. Virtually every C/C++ library for handling images comes with its own data structures for representing images. *I.e.* when trying to use more than one C/C++ library at a time, one ends up writing a lot of code for converting between different representation of images.

<span class="center"><img src="/pics/otsuin.jpg" width="320" alt=""/></span>

It get's worse. CPUs usually offer arithmetic using 8-bit, 16-bit, 32-bit, and 64-bit integers which can be signed or unsigned. Also there are single-precision and double-precision floating point numbers (*i.e.* 10 or more different native data types). When implementing a C/C++ library which just wants to support basic binary operations (addition, subtraction, division, multiplication, exponent, comparisons, ...) for array-scalar, scalar-array, and array-array combinations, one quickly ends up with literally thousands of possible combinations. This leads to a combinatorial explosion of methods as one can see in the [Framewave][3] library for example.

<span class="right"><a href="http://www.wedesoft.de/hornetseye-api/"><img src="/pics/hornetseye.jpg" width="100" alt=""/></a></span>
In the end I wrote a thesis about a different way of implementing machine vision systems. The thesis shows how one can implement machine vision software using a popular dynamically typed programming language (*i.e.* the Ruby programming language).

The listing below shows an IRB (Interactive Ruby) session to illustrate the result. Comment lines (preceded with '#') show the output of the IRB REPL (read-eval-print loop). The session first opens a video display showing the camera image. After closing the window it shows a video display with the thresholded camera image.

{% highlight ruby %}
require 'rubygems'
require 'hornetseye_v4l2'
require 'hornetseye_xorg'
include Hornetseye
input = V4L2Input.new
# #<Hornetseye::V4L2Input:0x7fbad6151a38>
X11Display.show { input.read }
# Frame(YUY2)(640,480,0x3fdd6b1972a0)
X11Display.show { (input.read.to_ubyte >= 128).conditional 255, 0 }
# MultiArray(UBYTE,2):
# [ [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .... ],
#   ....
{% endhighlight %}

See the picture below for an example of a thresholded image.
<span class="center"><img src="/pics/otsuout.png" width="320" alt=""/></span>

The example has just 7 lines of code. The REPL furthermore facilitates experimentation with machine vision software in an unprecedented way. The system achieves real-time by generating C-programs for the required operations, compiling them to Ruby extensions, and linking them on-the-fly.

I released the software as software libre under the name [Hornetseye][4]. My thesis is available for download now, too: [Efficient Implementations of Machine Vision Algorithms using a Dynamically Typed Programming Language][5] ([Bibtex][13]).

<span class="center"><a href="http://www.wedesoft.de/downloads/thesis_wedekind.pdf"><img src="/pics/thesis.png" width="196" alt=""/></a></span>

Here's the abstract:

<blockquote>
<p>Current machine vision systems (or at least their performance critical parts) are predominantly implemented using statically typed programming languages such as C, C++, or Java. Statically typed languages however are unsuitable for development and maintenance of large scale systems.</p>
<p>When choosing a programming language, dynamically typed languages are usually not considered due to their lack of support for high-performance array operations. This thesis presents efficient implementations of machine vision algorithms with the (dynamically typed) Ruby programming language. The Ruby programming language was used, because it has the best support for meta-programming among the currently popular programming languages. Although the Ruby programming language was used, the approach presented in this thesis could be applied to any programming language which has equal or stronger support for meta-programming (e.g. Racket (former PLT Scheme)).</p>
<p>A Ruby library for performing I/O and array operations was developed as part of this thesis. It is demonstrated how the library facilitates concise implementations of machine vision algorithms commonly used in industrial automation. That is, this thesis is about a different way of implementing machine vision systems. The work could be applied to prototype and in some cases implement machine vision systems in industrial automation and robotics.</p>
<p>The development of real-time machine vision software is facilitated as follows</p>
<ol>
<li>A just-in-time compiler is used to achieve real-time performance. It is demonstrated that the Ruby syntax is sufficient to integrate the just-in-time compiler transparently.</li>
<li>Various I/O devices are integrated for seamless acquisition, display, and storage of video and audio data.</li>
</ol>
<p>In combination these two developments preserve the expressiveness of the Ruby programming language while providing good run-time performance of the resulting implementation.</p>
<p>To validate this approach, the performance of different operations is compared with the performance of equivalent C/C++ programs.</p>
</blockquote>

I hope that my work has shown that the choice of programming language plays a fundamental role in the implementation of machine vision systems and that those choices should be revisited.

<h4>See also</h4>
* [HornetsEye][4]: Ruby computer vision library (developed as part of this thesis)
* [OpenCV][9]: C/C++ real-time computer vision library
* [NumPy][10]: Python numerical arrays
* [NArray][7]: Ruby numerical arrays
* [Lush][8]: Lisp dialect for large-scale numerical and graphic applications
* [Halide][6]: a language for image processing and computational photography
* [Maru][11]: a symbolic expression evaluator that can compile its own implementation language

<h4>Update</h4>
The thesis is now also [available on Figshare.com][12].

[1]: http://www2.imperial.ac.uk/robotvision/website/php/videos.php
[2]: http://fourcc.org/
[3]: http://framewave.sourceforge.net/
[4]: http://www.wedesoft.de/hornetseye-api/
[5]: http://www.wedesoft.de/downloads/thesis_wedekind.pdf
[6]: http://halide-lang.org/
[7]: http://narray.rubyforge.org/
[8]: http://lush.sourceforge.net/
[9]: http://opencv.willowgarage.com/
[10]: http://numpy.scipy.org/
[11]: http://piumarta.com/software/maru/
[12]: http://dx.doi.org/10.6084/m9.figshare.97244
[13]: http://www.wedesoft.de/downloads/thesis_wedekind.bib

