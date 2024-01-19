---
layout: post
title: Specifying Clojure function schemas with Malli
category: software
image: /pics/malli.png
---

[Clojure][2] is a dynamically typed language which means that functions can be called with arbitrary values.
If the function cannot operate on the values (or if it calls a function which cannot operate on a value or part of a value) a runtime exception is thrown.
Even worse, a function could exhibit undefined behaviour for the values provided.

Statically typed languages are often considered safer because the types of values are known at compile time so that the validity of values can be checked.
However statically typed implementations tend to become inflexible.
Type checking at compile time requires the types of the values held by variables to be defined at compile time.
When trying to keep the system both modular and extensible, the developer is forced to come up with complicated type hierarchies and possibly even generic types.

There is a promising solution to this and this is using pre- and post-conditions (contracts) for functions.
In particular there is [Malli][1] which allows you to specify schemas for each function using a concise syntax.
Malli schemas can be defined, reused, and composed like types.
Note that a value can fulfill multiple schemas (similar to Rust traits), however implementations and schemas exist independently from each other.
This allows schemas to be more specific than static types will ever be (e.g. you can have a schema for lists with three or more elements).

Malli is optimized for good runtime performance.
If performance is still a concern, one can leave schema validation disabled in production for some or all namespaces.

Here follows a **short introduction** to using Malli for function pre- and post-conditions:

You can use the Malli provider module to suggest possible schemas matching a list of values.

{% highlight clojure %}
(require '[malli.provider :as mp])
(mp/provide [1])
; :int
(mp/provide [1 nil])
; [:maybe :int]
(mp/provide [[1 2 3]])
; [:vector :int]
{% endhighlight %}

The resulting schemas can be used to validate values.

{% highlight clojure %}
(require '[malli.core :as m])
(m/validate [:vector :int] [1 2 3])
; true
(m/validate [:vector :int] [1 2 nil])
; false
{% endhighlight %}

Schemas are values which can be reused and composed.

{% highlight clojure %}
(def N (m/schema [:int {:min 1}]))
(m/validate [:vector N] [0 1 2])
; false
(m/validate [:vector N] [1 2 3])
; true
{% endhighlight %}

It is possible to add predicate functions to schemas.

{% highlight clojure %}
(def ordered-vector (m/schema [:and [:vector :int] [:fn (fn [v] (apply < v))]]))
(m/validate ordered-vector [2 5 3])
; false
(m/validate ordered-vector [2 3 5])
; true
{% endhighlight %}

Most importantly one can add pre- and post-conditions ([function schemas][3]) to functions.
{% highlight clojure %}
(defn sqr
  {:malli/schema [:=> [:cat :double] :double]}
  [x]
  (* x x))
{% endhighlight %}

By default function schemas are ignored until the schemas are collected and the functions are instrumented.
Note that here I am replacing the default exception thrower with a more human-readable one.

{% highlight clojure %}
(require '[malli.instrument :as mi])
(require '[malli.dev.pretty :as pretty])
(mi/collect!)
(mi/instrument! {:report (pretty/thrower)})
(sqr 3.0)
; 9.0
(sqr "text")
; -- Schema Error ---------------------------------------------------------------- NO_SOURCE_FILE:1 --
;
; Invalid function arguments:
;
;   ["text"]
;
; Function Var:
;
;   user/sqr
;
; Input Schema:
;
;   [:cat :double]
;
; Errors:
;
;   {:in [0], :message "should be a double", :path [0], :schema :double, :value "text"}
;
; More information:
;
;   https://cljdoc.org/d/metosin/malli/CURRENT/doc/function-schemas
;
; ----------------------------------------------------------------------------------------------------
{% endhighlight %}

If you want to collect the schemas from a module (e.g. for testing purposes), you can specify the namespace as follows.

{% highlight clojure %}
(mi/collect! {:ns ['the.module]})
{% endhighlight %}

You can also parse code to infer a general schema for a function header.

{% highlight clojure %}
(require '[malli.destructure :as md])
(def infer (comp :schema md/parse))
(infer '[x])
; [:cat :any]
{% endhighlight %}

If you have a multiple arity function, you can use *:function* to specify the different alternatives.

{% highlight clojure %}
(defn f
  {:malli/schema [:function [:=> [:cat :double] :double]
                            [:=> [:cat :double :double] :double]]}
  ([x] x)
  ([x y] (+ x y)))
(mi/collect!)
(mi/instrument! {:report (pretty/thrower)})
(f 2.0 3.0)
; 5.0
(f "test")
; ... error ...
{% endhighlight %}

There is also support for [sequence schemas][4] which allows a more compact schema in this case.

{% highlight clojure %}
(defn f
  {:malli/schema [:=> [:cat :double [:? :double]] :double]}
  ([x] x)
  ([x y] (+ x y)))
{% endhighlight %}

Finally here is an example for a method accepting keyword arguments which took me some time to figure out.

{% highlight clojure %}
(defn g
  {:malli/schema [:=> [:cat [:= :x] :double [:= :y] :double] :double]}
  [& {:keys [x y]}]
  (+ x y))
(mi/collect!)
(mi/instrument! {:report (pretty/thrower)})
(g :x 2.0 :y 3.0)
; 5.0
(g :x 2.0 :y "test")
; ... error ...
{% endhighlight %}

The only problem I encountered so far is that instrumentation clashes with Clojure [type hints][5].
I guess the solution is to instead provide the type hints at the call site instead where necessary for performance reasons.

Enjoy!

**Updates:**

You can also define [recursive schemas][6] by using a local registry:

{% highlight clojure %}
(require '[malli.core :as m])
(def nested-vector
  (m/schema [:schema {:registry {::node [:or :int [:vector [:ref ::node]]]}}
                     [:ref ::node]]))
(m/validate nested-vector [[1 2] 3])
; true
(m/validate nested-vector [[1 :k] 3])
; false
{% endhighlight %}

One can use *m/=>* to add a Malli schema to a function.
This comes in handy when you need to add a schema to a Clojure multimethod.

{% highlight clojure %}
(require '[malli.core :as m])
(require '[malli.instrument :as mi])
(require '[malli.dev.pretty :as pretty])
(defmulti factorial identity)
(m/=> factorial [:=> [:cat integer?] integer?])
(defmethod factorial 0 [_]  1N)
(defmethod factorial :default [num]
   (* num (factorial (dec num))))
(mi/instrument! {:report (pretty/thrower)})
(factorial 10)
; 3628800N
(factorial 1.5)
; ... error ...
{% endhighlight %}

[1]: https://github.com/metosin/malli
[2]: https://clojure.org/
[3]: https://github.com/metosin/malli/blob/master/docs/function-schemas.md
[4]: https://github.com/metosin/malli#sequence-schemas
[5]: https://clojure.org/reference/java_interop#typehints
[6]: https://github.com/metosin/malli#recursive-schemas
