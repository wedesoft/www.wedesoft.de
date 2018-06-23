---
layout: post
title: Clojure for Schemers
thumb: /pics/clojure.png
---

{% vimeo 22675078 %}

Recently I found a video of an interesting presentation by *David Nolen* ([homepage][9], [blog][10]) (comparing the [Clojure][1] programming language with [Scheme (Racket)][2]. Some important points made in the talk are:

* [The Little Schemer][4], [The Seasoned Schemer][5], and [The Reasoned Schemer][8] are great Q&A-style books for getting to grips with the Scheme programming language.
* Clojure does not support optimisation of tail-recursion leading to stack overflows. However one can use lazy sequences instead.
* Clojure does not support continuations.
* The frequent destructuring of data using **car** and **cdr** in Scheme is cumbersome. Clojure on the other hand has syntax support for various immutable data structures (lists, vectors, hashes, and sets) which makes for more readable code.
* Clojure is a great platform for exploring concurrency (atoms, refs, agents, promises, and futures).

David Nolen concludes that it is not a question of either/or.

**See also:**

* Web pages
  * [Clo*j*ure programming language][1]
  * [Racket programming language][2]
* Books
  * [Structure and Interpretation of Computer Programs][6]
  * [The Little Schemer][4]
  * [The Seasoned Schemer][5]
  * [The Reasoned Schemer][8]
  * [The Joy of Clojure][3]
* Software
  * [Maru metacircular s-expression evaluator and compiler][13]
* Talks
  * [David Nolen - An Introduction to Lisp Macros][11]
  * [Guy Steele - Growing a Language][12]

[1]: http://clojure.org/
[2]: http://racket-lang.org/
[3]: http://joyofclojure.com/
[4]: http://www.ccs.neu.edu/home/matthias/BTLS/
[5]: http://www.ccs.neu.edu/home/matthias/BTSS/
[6]: http://sicpebook.wordpress.com/
[7]: http://htdp.org/
[8]: http://www.ccs.neu.edu/home/matthias/BRS/
[9]: http://itp.nyu.edu/~dn525/
[10]: http://dosync.posterous.com/
[11]: http://vimeo.com/23907627
[12]: http://video.google.com/videoplay?docid=-8860158196198824415
[13]: http://piumarta.com/software/maru/
