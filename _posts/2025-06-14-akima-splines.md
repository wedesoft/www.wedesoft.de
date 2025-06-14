---
layout: post
title: Akima splines
category: software
image: /pics/connascence.jpg
---

Recently I was looking for spline interpolation for creating curves from a set of samples.
I knew cubic splines which are piecewise cubic polynomials fitted such that they are continuous up to the second derivative.
I almost went ahead and implemented cubic splines using a matrix solver but then I found that the [fastmath][1] Clojure library already provides splines.
The [fastmath][1] spline interpolation module is based on the [interpolation module of the Java Smile library][2].
I saved the interpolated samples to a text file and plotted them with [Gnuplot][3].

{% highlight clojure %}
(require '[fastmath.interpolation :as interpolation])
(use '[clojure.java.shell :only [sh]])

(def px [0 1 3 4 5 8 9])
(def py [0 0 1 7 3 4 6])
(spit "/tmp/points.dat" (apply str (map (fn [x y] (str x " " y "\n")) px py)))

(def cspline (interpolation/cubic-spline px py))
(def x (range 0 8.0 0.01))
(spit "/tmp/cspline.dat" (apply str (map (fn [x y] (str x " " y "\n")) x (map cspline x))))
(sh "gnuplot" "-c" "plot.gp" "/tmp/cspline.png" "/tmp/cspline.dat")
(sh "display" "/tmp/cspline.png")
{% endhighlight %}

I used the following Gnuplot script *plot.gp* for plotting:

{% highlight gnuplot %}
set terminal pngcairo size 640,480
set output ARG1
set xlabel "x"
set ylabel "y"
plot ARG2 using 1:2 with lines title "spline", "/tmp/points.dat" using 1:2 with points title "points"
{% endhighlight %}

I used a lightweight configuration of the *fastmath* library without MKL and OpenBLAS.
See following *deps.edn*:

{% highlight clojure %}
{:deps {org.clojure/clojure {:mvn/version "1.12.1"}
        generateme/fastmath {:mvn/version "2.4.0" :exclusions [com.github.haifengl/smile-mkl org.bytedeco/openblas]}}}
{% endhighlight %}

The result is shown in the following figure.
One can see that the spline is smooth and passes through all points, however it shows a high degree of oscillation:

![cubic spline](/pics/cspline.png)

However I found another spline algorithm in the *fastmath* wrappers: The [Akima spline][4].
The Akima spline needs at least 5 points and it first computes the gradient of the lines connecting the points.
Then for each point it uses a weighted average of the previous and next slope value.
The slope values are weighted using the absolute difference of the previous two slopes and the next two slopes, i.e. the curvature.
The first and last two points use a special formula:
The first and last point use the next or previous slope and the second and second last point use an average of the neighbouring slopes.

{% highlight clojure %}
(require '[fastmath.interpolation :as interpolation])
(use '[clojure.java.shell :only [sh]])

(def px [0 1 3 4 5 8 9])
(def py [0 0 1 7 3 4 6])
(spit "/tmp/points.dat" (apply str (map (fn [x y] (str x " " y "\n")) px py)))

(def aspline (interpolation/akima-spline px py))
(def x (range 0 8.0 0.01))
(spit "/tmp/aspline.dat" (apply str (map (fn [x y] (str x " " y "\n")) x (map aspline x))))
(sh "gnuplot" "-c" "plot.gp" "/tmp/aspline.png" "/tmp/aspline.dat")
(sh "display" "/tmp/aspline.png")
{% endhighlight %}

![Akima spline](/pics/aspline.png)

So if you have a data set which causes cubic splines to oscillate, give Akima splines a try!

Enjoy!

[1]: https://github.com/generateme/fastmath
[2]: https://haifengl.github.io/interpolation.html
[3]: http://www.gnuplot.info/
[4]: https://en.wikipedia.org/wiki/Akima_spline
