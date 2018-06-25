---
layout: post
title: OOP with GNU Guile and GOOPS
category: software
image: /pics/guile.png
---

Having worked with the Ruby programming language for many years I started to get
interested in Scheme. The Scheme programming language has a small and powerful
core supporting closures, hygienic macros, first-class continuations, and of
course the famous interpreter functions *eval* and *apply*.

<span class="center"><a href="/pics/lisp-programmers.jpg"><img src="/pics/lisp-programmers-small.jpg" width="400" alt="How a Common Lisp programmer views users of other languages"/></a></span>

However one thing I don't like about Scheme is that there are different function
names for each type of arguments. *E.g.* adding numbers is done with **+**, adding
lists is done with **append**, and adding strings is done with **string-append**.

{% highlight scheme %}
(+ 2 3)
; 5
(append '(1) '(2 3))
; (1 2 3)
(string-append "a" "bc")
; "abc"
{% endhighlight %}

The same program would be much less verbose if **+** was extended to work with
strings and lists, too:

{% highlight scheme %}
(+ 2 3)
; 5
(+ '(1) '(2 3))
; (1 2 3)
(+ "a" "bc")
; "abc"
{% endhighlight %}

<h3>GOOPS: The Guile extension for object-oriented programming</h3>

<span class="right"><a href="http://www.gnu.org/software/guile/"><img src="/pics/guile.png" width="200" alt="GNU Guile"/></a></span>
It turns out that GNU Guile (the Scheme interpreter of the GNU project) has
a mature extension which facilitates this. [GOOPS][6] is inspired by the
[Common Lisp Object System (CLOS)][5]. GOOPS not only provides polymorphic
methods, it even lets you replace existing functions with polymorphic ones:

{% highlight scheme %}
(use-modules (oop goops))
(define-method (+ (a <list>) (b <list>)) (append a b))
(define-method (+ (a <string>) (b <string>)) (string-append a b))
(+ 2 3)
; 5
(+ '(1) '(2 3))
; (1 2 3)
(+ "a" "bc")
; "abc"
{% endhighlight %}

The *define-class* method is the normal way to define a class. GOOPS requires
you to define the instance attributes when defining the class. The following
example defines a class **\<a\>** with attribute **x** and a **+** method. The
**make** method is used to create the instance **a** of the class **\<a\>**.

{% highlight scheme %}
(use-modules (oop goops))
(define-class <a> ()
  (x #:init-value 0 #:init-keyword #:x #:getter get-x))
(define-method (+ (u <a>) (v <a>))
  (make <a> #:x (+ (get-x u) (get-x v))))
<a>
; #<<class> <a> 2103c30>
(define a (make <a> #:x 123))
a
; #<<a> 22a2440>
(get-x a)
; 123
(+ a a)
; #<<a> 23713e0>
(get-x (+ a a))
; 246
{% endhighlight %}

<h3>Constructors and the next method</h3>

<span class="center"><img src="/pics/god-uses-lisp.jpg" width="600" alt="God uses Lisp"/></span>

One can define a shorthand for instantiating objects. *E.g.* one can define a
method which sets the attribute **#:x** to the argument multiplied by two.

{% highlight scheme %}
(use-modules (oop goops))
(define-class <a> ()
  (x #:init-value 0 #:init-keyword #:x #:getter get-x))
(define (make-a x2) (make <a> #:x (* 2 x2)))
(define a2 (make-a 123))
; #<<a> 1689740>
(get-x a2)
; 246
{% endhighlight %}

IMHO it is better though to define a modified constructor directly. This is more
involved but also possible.

{% highlight scheme %}
(use-modules (oop goops) (ice-9 optargs))
(define-class <a> ()
  (x #:init-value 0 #:init-keyword #:x #:getter get-x))
(define-method (initialize (self <a>) initargs)
  (let-keywords initargs #f (x2)
    (next-method self (list #:x (* 2 x2)))))
(define a3 (make <a> #:x2 123))
(get-x a3)
; 246
{% endhighlight %}

Note the call to **next-method**. This essentially calls the
[next less specialized method][7] for that generic function. Here is another
example using an inheriting class **\<b\>** to illustrate the concept.

{% highlight scheme %}
(use-modules (oop goops))
(define-class <a> ())
(define-class <b> (<a>))
(define-method (test (self <a>)) "a")
(define-method (test (self <b>)) (string-append (next-method) "b"))
(test (make <a>))
; "a"
(test (make <b>))
; "ab"
{% endhighlight %}

<h3>Metaclasses</h3>

<span class="center"><img src="/pics/meta.jpg" width="325" alt="Metaprogramming"/></span>

Note that GOOPS does not implicitly create a metaclass. The following example
shows how to define a metaclass **\<m\<c\>\>** with a class method.

{% highlight scheme %}
(use-modules (oop goops))
(define-class <m<c>> (<class>))
(define-method (test <m<c>>) "m")
(define-class <c> ()
  (x #:init-keyword #:x #:getter get-x) #:metaclass <m<c>>)
(define o (make <c> #:x 5))
o
; #<<c> 2825d40>
(test (class-of o))
; "m"
{% endhighlight %}

<h3>Inspection</h3>

<span class="center"><img src="/pics/inspection.jpg" width="448" alt="Inspection"/></span>

One can also use GOOPS to change the way how objects get displayed and what the
*REPL* writes to the terminal. *E.g.* one can define the method **write** for
the class **\<a\>** to change the way the Guile REPL displays objects of that
class.

{% highlight scheme %}
(use-modules (oop goops))
(define-class <a> ()
  (x #:init-value 0 #:init-keyword #:x #:getter get-x))
(define a (make <a> #:x 5))
a
; #<<a> 2c64140>
(define-method (write (self <a>) port)
  (format port "(make <a> #:x ~a)" (get-x self)))
a
; (make <a> #:x 5)
{% endhighlight %}

Furthermore one can implement the method **display** to define the way objects
get displayed in formatted output.

{% highlight scheme %}
(define-method (display (self <a>) port)
  (format port "*~a*" (get-x self)))
(format #t "~a~&" a)
; *5*
{% endhighlight %}

<h3>Conclusion</h3>

I have now [used GOOPS][8] for a little while. Coming from Ruby there are a few
gotchas when using GOOPS and Guile's module system. For example one needs to use
a **#:re-export** statement when using a module to replace the core binding for
the **+** operator.

All in all GOOPS makes quite a mature impression and I think it makes Scheme much
more amenable to developers who are used to thinking in terms of objects and
classes.

To quote Guile's foreign function interface documentation:

<blockquote>
<p>The more one hacks in Scheme, the more one realizes that there are
actually two computational worlds: one which is warm and alive, that
land of parentheses, and one cold and dead, the land of C and its ilk.</p>
</blockquote>

Any comments and suggestions are welcome <img src="/pics/bounce.gif" width="19" alt=""/>

<h3>Further remarks</h3>

If necessary it is also possible to create objects, classes, and metaclasses
dynamically. The following example instantiates the object **v** of class
**\<v\>** of metaclass **\<m\<v\>\>**. Furthermore the generic **test** is
implemented for **\<v\>**.

{% highlight scheme %}
(use-modules (oop goops))
(define-generic test)
(define <m<v>> (make <class>
                     #:dsupers `(,<class>)
                     #:slots '()
                     #:name "<m<v>>"))
(define <v> (make <m<v>>
                  #:dsupers `(,<object>)
                  #:slots '()
                  #:name "<v>"))
(add-method! test (make <method>
                        #:specializers `(,<v>)
                        #:procedure (lambda (self) "v")))
(define v (make <v>))
v
; #<<v> 2d5a750>
(test v)
; "v"
{% endhighlight %}

**Update:**

I [tweeted][12] about the article and submitted it to [Hackernews][10] and the
[GUILE user list][11].

I have reduced the use of *slot-ref* during a discussion on the [GUILE user list][16].

**See also:**

* [GNU Guile][9]
* [GOOPS][6]
  * [Generic functions and methods example][13]
  * [Guile object properties][17]
* [Structure and Interpretation of Computer Programs][1]
* [The Little Schemer][2]
* [The Seasoned Schemer][3]
* [The Reasoned Schemer][4]
* [COOPS object system for Chicken Scheme][14]
* [Clojure multi-methods][15]

[1]: http://sicpebook.wordpress.com/ebook/
[2]: http://www.ccs.neu.edu/home/matthias/BTLS/
[3]: http://www.ccs.neu.edu/home/matthias/BTSS/
[4]: http://www.ccs.neu.edu/home/matthias/BRS/
[5]: https://en.wikipedia.org/wiki/Common_Lisp_Object_System
[6]: https://www.gnu.org/software/goops/
[7]: http://www.gnu.org/software/guile/docs/goops/Next-method.html
[8]: https://github.com/wedesoft/aiscm
[9]: http://www.gnu.org/software/guile/
[10]: https://news.ycombinator.com/item?id=7331142
[11]: http://lists.gnu.org/archive/html/guile-user/2014-03/msg00000.html
[12]: https://twitter.com/wedesoft/status/440568251748073473
[13]: http://www.gnu.org/software/guile/manual/html_node/Generic-Function-and-Method-Examples.html
[14]: http://wiki.call-cc.org/eggref/4/coops
[15]: http://clojure.org/reference/multimethods
[16]: http://lists.gnu.org/archive/html/guile-user/2016-03/msg00067.html
[17]: https://www.gnu.org/software/guile/manual/html_node/Object-Properties.html
