---
layout: post
title: Performance comparison of Clojure, Ruby, and Python
category: software
image: /pics/speedometer.png
---

## Introduction

![Speedometer](/pics/speedometer.png)

A fair performance comparison of implementations of different programming languages is difficult.
Ideally one measures the performance for a wide range of algorithms and programming tasks.
However each language will have different application areas where it performs best.

In the following we compare the performance of Clojure, Ruby, and Python on the `factorial` function.
While the results might not allow to make general statements about the performance of each language interpreter, it gives us an idea of what the individual strengths and weaknesses are.

The measurements were performed using an AMD Ryzen 7 4700U with 16GB RAM and 8 cores.
* For the Clojure measurements I used the [Criterium][2] benchmarking library.
* For the Ruby measurements I used the benchmark module of the standard library.
* For the Python measurements I used the [Pyperf][3] module.
See this [Gist][1] for the benchmarking source code.

## Implementations

There are different ways to implement the `factorial` function.

### Recursive

In functional programming the recursive implementation of the `factorial` function is the most common.

The Clojure implementation is as follows.
{% highlight clojure %}
(defn factorial [n]
  (if (zero? n)
    1
    (*' n (factorial (dec n)))))
{% endhighlight %}
Note that we use `*'` instead of `*` which is required if we want to be able to handle large numbers.

The Ruby implementation is as follows.
{% highlight ruby %}
def factorial n
  return 1 if n <= 1
  n * factorial(n - 1)
end
{% endhighlight %}

Finally the Python implementation is as follows.
{% highlight python %}
def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n - 1)
{% endhighlight %}

### Loop

If we introduce an additional variable, we can implement the `factorial` using a loop.

In Clojure we can use tail recursion as follows.
{% highlight clojure %}
(defn factorial [n]
  (loop [result 1N n n]
    (if (zero? n)
      result
      (recur (*' result n) (dec n)))))
{% endhighlight %}
Note that we need to initialise the result with `1N` (big number type) instead of `1`, because the rebinding of the result variable does not allow a dynamic type change.

In Ruby we can implement `factorial` using a `while` loop.
{% highlight ruby %}
def factorial n
  result = 1
  while n.positive?
    result *= n
    n -= 1
  end
  result
end
{% endhighlight %}

In Python we can implement `factorial` using a `while` loop as well.
{% highlight python %}
def factorial(n):
    result = 1
    while n > 0:
        result *= n
        n -= 1
    return result
{% endhighlight %}

### Reduce

Using the higher-order function `reduce` we can implement the `factorial` as follows.

In Clojure:
{% highlight clojure %}
(defn factorial [n] (reduce *' (range 1 (inc n))))
{% endhighlight %}

In Ruby:
{% highlight ruby %}
def factorial n
  1.upto(n).reduce :*
end
{% endhighlight %}

And in Python:
{% highlight python %}
def factorial(n):
    return reduce(operator.mul, range(1, n + 1))
{% endhighlight %}

### Unchecked integer math

One can use unchecked integers and type annotations in Clojure if it is known, that the result is not going to exceed the integer range.
{% highlight clojure %}
(set! *unchecked-math* true)
(defn factorial [^long n] (if (zero? n) 1 (* n (factorial (dec n)))))
(set! *unchecked-math* false)
{% endhighlight %}

A similar approach in Python for improving performance is to use the Cython compiler.
Here the method is implemented in a dialect of Python which uses static typing.
{% highlight python %}
def factorial(int n):
    cdef int i, ret
    ret = 1
    for i in range(n):
        ret *= n
    return ret
{% endhighlight %}

Ruby only has the [RubyInline library][4] which requires to reimplement the method in C.
{% highlight ruby %}
class Factorial
  inline do |builder|
    builder.c "
        long factorial(int max) {
          int i=max, result=1;
          while (i >= 2) { result *= i--; }
          return result;
        }"
  end
end
{% endhighlight %}

### Other implementations

In Python one can instead use the `factorial` from the `math` module.

In Clojure one can apply the multiplication function to a range of numbers, since multiplication in Clojure can take an arbitrary number of arguments.
{% highlight clojure %}
(defn factorial [n] (apply *' (range 1 (inc n))))
{% endhighlight %}

Only Clojure supports parallelism.
For computing factorials we can use the `fold` function.
Here we split the task into two chunks.
Unfortunately `range` does not support random access, so we need to convert it to a vector.
{% highlight clojure %}
(defn factorial4 [n] (clojure.core.reducers/fold (quot n 2) *' *' (vec (range 1 (inc n)))))
{% endhighlight %}

Finally if the input argument is known at compile time, one can use a macro in Clojure.
This obviously is going to have much better performance than all the other implementations.
{% highlight clojure %}
(defmacro factorial-macro [n]
  `(fn [] (*' ~@(range 1 (inc n)))))
{% endhighlight %}

## Factorial of 20

First we compared the performance of computing the factorial of 20.

| implementation    | Clojure 1.12.0 | Ruby 3.4.1 | Python 3.13.1 |
|-------------------|----------------|------------|---------------|
| recursive         |     **104 ns** |     538 ns |        957 ns |
| loop              |     **164 ns** |     665 ns |        717 ns |
| reduce            |     **116 ns** |    1512 ns |        718 ns |
| unchecked integer |        44.4 ns |    77.6 ns |   **41.6 ns** |
| fold              |    **6211 ns** |        n/a |           n/a |
| math library      |            n/a |        n/a |   **45.4 ns** |
| apply             |     **178 ns** |        n/a |           n/a |
| macro             |   **0.523 ns** |        n/a |           n/a |

The Clojure implementation makes use of the JVM and the resulting performance for recursive, loop, and reduce implementation of factorial is the best.
Forcing `fold`, which is a parallel version of `reduce`, to use 2 threads, does not yield better performance.

Note that the recursive implementation of Ruby is faster than the Python implementation.
This is maybe due to the YJIT optimizing JIT compiler build into the Ruby interpreter.

Also note that the loop implementation in Python is faster than the recursive implementation.
Surprisingly the reduce implementation in Python has comparable performance to the loop implementation.

The factorial implementation of the Python math library is very fast.
The Python math library implementation uses a lookup table for arguments up to 20.

Since the result of factorial of 20 fits into a 64 bit integer, one can use unchecked integers in Clojure to get a fast implementation.
The resulting implementation is slightly faster than the Python math library implementation.
Note that the factorial implementation of the Python math library does not perform unchecked 64 bit integer math.
In Python one can use the Cython dialect to use the C compiler and unchecked math with 64 bit integers.
Finally one can use RubyInline to embed a C implementation in a Ruby program.
The Cython implementation is the fastest for computing factorial of 20.

Finally using a macro, which of the three languages only Clojure supports, is faster than all other implementations but obviously limited to cases where the function argument is known at compile time.

## Factorial of 100

When computing the factorial of arguments greater than 20, big integers are required.
In the following table the performance of computing factorials of 100 is shown.

| implementation | Clojure 1.12.0 | Ruby 3.4.1 | Python 3.13.1 |
|----------------|----------------|------------|---------------|
| recursive      |    **3148 ns** |   27191 ns |       5520 ns |
| loop           |    **3434 ns** |   32426 ns |       4430 ns |
| reduce         |    **2592 ns** |   28851 ns |       3790 ns |
| fold           |   **16012 ns** |        n/a |           n/a |
| math library   |            n/a |        n/a |    **599 ns** |
| apply          |    **2624 ns** |        n/a |           n/a |
| macro          |    **3.70 ns** |        n/a |           n/a |

Again the recursive, loop, and reduce implementations are fastest in Clojure.
Also again using fold with two chunks fares much worse.

The Ruby implementations fare much worse for big numbers.
Maybe the implementation of big numbers in Ruby has much worse performance than the Python and Clojure one.

Overall the implementation in the Python math library is the fastest candidate (unless we can use a Clojure macro of course).

## Conclusion

As stated before, one cannot generalize this limited performance comparison.
However maybe one can maybe make the following observations:
* The combination of Clojure and the JVM allows for better performance of dynamically typed programs.
* The performance of numerical algorithms in Clojure can be further improved when using unchecked math. The performance in this case is even getting close to Cython.
* Being able to call an AOT compiled C-implementation of a numerical algorithm still gives the best performance as can be seen by the Python math library implementation of factorial. The fact that this method exists is maybe a reflection of the fact that Python has the strongest support when it comes to numerical libraries.
* Only Clojure supports macros, which allow for results to be computed at compile time instead of at runtime, if the arguments are known early.
* Currently only Clojure supports parallel algorithms such as `fold` and `pmap`. However they only offer performance benefits for larger tasks than the one tested here.

Any suggestions and comments are welcome.

**Updates:**

* Replaced Numba implementation with Cython.
* Add type hints to unchecked math Clojure implementation.
* Add RubyInline implementation.

Because Python and Ruby bind methods late, there is a significant overhead when calling methods.
In the following the identity function is tested in Clojure, Ruby, and Python.
It looks like the JVM has even inlined the Clojure method, because the method invocation time is close to zero.

| implementation    | Clojure 1.12.0 | Ruby 3.4.1 | Python 3.13.1 |
|-------------------|----------------|------------|---------------|
| identity function |  **0.0023 ns** |    42.5 ns |       44.2 ns |

Also see: [https://clojure-goes-fast.com/](https://clojure-goes-fast.com/)

[1]: https://gist.github.com/wedesoft/f72020437ce035a394c0e12c2208f8b3
[2]: https://cljdoc.org/d/criterium/criterium/
[3]: https://pyperf.readthedocs.io/
[4]: http://docs.seattlerb.org/RubyInline/
