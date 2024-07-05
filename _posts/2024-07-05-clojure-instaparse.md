---
layout: post
title: Implement an Interpreter using Clojure Instaparse
category: software
image: /pics/clojure.png
---

This is a small example on how to implement an interpreter using [Clojure][1] and the [Instaparse][2] library.

#### Dependencies

First we create a *deps.edn* file to get Rich Hickey's [Clojure][1], Mark Engelberg's [Instaparse][2], the [Midje][3] test suite by Brian Marick, and my modified version of Max Miorim's [midje-runner][5]:

{% highlight clojure %}
{:deps {org.clojure/clojure {:mvn/version "1.11.3"}
        instaparse/instaparse {:mvn/version "1.5.0"}}
 :paths ["src"]
 :aliases
 {:test
  {:extra-deps
   {midje/midje {:mvn/version "1.10.9"}
    midje-runner/midje-runner
    {:git/url "https://github.com/wedesoft/midje-runner.git"
     :git/sha "8ceb29d5781b9fc43ad4116fc645ade797342fad"}}
   :extra-paths ["test"]
   :main-opts ["-m" "midje-runner.runner"]}}}
{% endhighlight %}

#### Initial setup

Next we create a test suite with an initial test in *test/clj_calculator/t_core.clj*:

{% highlight clojure %}
(ns clj-calculator.t-core
  (:require [midje.sweet :refer :all]
            [instaparse.core :as insta]
            [clj-calculator.core :refer :all]))

(facts "Test parser"
       (calc-parser "-42") => [:START [:NUMBER "-42"]])
{% endhighlight %}

You can run the test suite as follows which should give an error.

{% highlight bash %}
clj -M:test
{% endhighlight %}

Next we create a module with the parser in *src/clj_calculator/core.clj*:

{% highlight clojure %}
(ns clj-calculator.core
    (:require [instaparse.core :as insta]))

(def calc-parser
  (insta/parser
    (slurp "resources/clj_calculator/calculator.bnf")))
{% endhighlight %}

We also need to create an initial grammar in *resources/clj_calculator/calculator.bnf* defining a minimal grammar and a regular expression for parsing an integer:
{% highlight text %}
START  = NUMBER
NUMBER = #'[-+]?[0-9]+'
{% endhighlight %}

At this point the first test should pass.

#### Ignoring whitespace

Next we add a test to ignore whitespace.

{% highlight clojure %}
(facts "Test parser"
       ; ...
       (calc-parser " -42 ") => [:START [:NUMBER "-42"]])
{% endhighlight %}

The test should fail with unexpected input.
The grammar needs to be modified to pass this test:

{% highlight text %}
START      = <WHITESPACE?> NUMBER <WHITESPACE?>
NUMBER     = #'[-+]?[0-9]+'
WHITESPACE = #'[,\ \t]+'
{% endhighlight %}

Note the use of '<' and '>' to omit the parsed whitespace from the parse tree.

#### Parsing expressions

Next we can add tests for sum, difference, or product of two numbers:

{% highlight clojure %}
(facts "Test parser"
       ; ...
       (calc-parser "1 + 2") => [:START [:SUM [:NUMBER "1"] [:NUMBER "2"]]]
       (calc-parser "5 - 4") => [:START [:DIFF [:NUMBER "5"] [:NUMBER "4"]]]
       (calc-parser "2 * 3") => [:START [:PROD [:NUMBER "2"] [:NUMBER "3"]]])
{% endhighlight %}

The grammar now becomes:

{% highlight text %}
START      = <WHITESPACE?> (NUMBER | SUM | DIFF | PROD) <WHITESPACE?>
SUM        = NUMBER <WHITESPACE?> <'+'> <WHITESPACE?> NUMBER
DIFF       = NUMBER <WHITESPACE?> <'-'> <WHITESPACE?> NUMBER
PROD       = NUMBER <WHITESPACE?> <'*'> <WHITESPACE?> NUMBER
NUMBER     = #'[-+]?[0-9]+'
WHITESPACE = #'[,\ \t]+'
{% endhighlight %}

#### Transforming syntax trees

Instaparse comes with a useful transformation function for recursively transforming the abstract syntax tree we obtained from parsing.
First we write and run a failing test for transforming a string to an integer:

{% highlight clojure %}
(facts "Test calculator"
       (calculate "-42") => -42)
{% endhighlight %}

To pass the test we implement a calculator function which transforms the syntax tree.
Initially it only needs to deal with the nonterminal symbols START and NUMBER:

{% highlight clojure %}
(defn calculate
  [string]
  (instaparse.transform/transform
    {:START  identity
     :NUMBER #(Integer/parseInt %)}
    (calc-parser string)))
{% endhighlight %}

#### Performing calculations

Obviously we can use the transformation function to also perform the calculations.
Here are the tests for the three possible operations of the parse tree.

{% highlight clojure %}
(facts "Test calculator"
       (calculate "-42") => -42
       (calculate "1 + 2") => 3
       (calculate "5 - 4") => 1
       (calculate "2 * 3") => 6)
{% endhighlight %}

The implementation using the Instaparse transformation function is quite elegant:

{% highlight clojure %}
(defn calculate
  [string]
  (instaparse.transform/transform
    {:START  identity
     :NUMBER #(Integer/parseInt %)
     :SUM    +
     :DIFF   -
     :PROD   *}
    (calc-parser string)))
{% endhighlight %}

#### Recursive Grammar

The next test is about implementing an expression with two operations.

{% highlight clojure %}
(facts "Recursive grammar"
       (calculate "2 - 1 + 3") => 4)
{% endhighlight %}

A naive implementation using a blind EXPR nonterminal symbol passes the test:

{% highlight text %}
START      = <WHITESPACE?> (EXPR | SUM | DIFF | PROD) <WHITESPACE?>
<EXPR>     = SUM | DIFF | PROD | NUMBER
SUM        = EXPR <WHITESPACE?> <'+'> <WHITESPACE?> EXPR
DIFF       = EXPR <WHITESPACE?> <'-'> <WHITESPACE?> EXPR
PROD       = EXPR <WHITESPACE?> <'*'> <WHITESPACE?> EXPR
NUMBER     = #'[-+]?[0-9]+'
WHITESPACE = #'[,\ \t]+'
{% endhighlight %}

However there is a problem with this grammar: It is ambiguous.
The following failing test shows that the parser could generate two different parse trees:

{% highlight clojure %}
(facts "Recursive grammar"
       ; ...
       (count (insta/parses calc-parser "2 - 1 + 3")) => 1)
{% endhighlight %}

When parsing small strings, this might not be a problem.
However if you use an ambiguous grammar to parse a large file with a syntax error near the end, the resulting combinatorial explosion leads to a long processing time before the parser can return the syntax error.
The good thing is, that Instaparse uses the GLL parsing algorithm, i.e. it can handle a left-recursive grammar to resolve the ambiguity:

{% highlight text %}
START      = <WHITESPACE?> (EXPR | SUM | DIFF | PROD) <WHITESPACE?>
<EXPR>     = SUM | DIFF | PROD | NUMBER
SUM        = EXPR <WHITESPACE?> <'+'> <WHITESPACE?> NUMBER
DIFF       = EXPR <WHITESPACE?> <'-'> <WHITESPACE?> NUMBER
PROD       = EXPR <WHITESPACE?> <'*'> <WHITESPACE?> NUMBER
NUMBER     = #'[-+]?[0-9]+'
WHITESPACE = #'[,\ \t]+'
{% endhighlight %}

This grammar is not ambiguous any more and will pass above test.

#### Grouping using brackets

We might want to use brackets to group expressions and influence the order expressions are applied:

{% highlight clojure %}
(facts "Recursive grammar"
       ; ...
       (calculate "2 - ( 1 + 3 )") => -2
       (calculate "( 2 - 1 ) + 3") => 4)
{% endhighlight %}

The following grammar implements this:

{% highlight text %}
START      = <WHITESPACE?> (EXPR | SUM | DIFF | PROD) <WHITESPACE?>
<EXPR>     = SUM | DIFF | PROD | GROUP
<GROUP>    = NUMBER | <'('> <WHITESPACE?> EXPR <WHITESPACE?>  <')'>
SUM        = EXPR <WHITESPACE?> <'+'> <WHITESPACE?> GROUP
DIFF       = EXPR <WHITESPACE?> <'-'> <WHITESPACE?> GROUP
PROD       = EXPR <WHITESPACE?> <'*'> <WHITESPACE?> GROUP
NUMBER     = #'[-+]?[0-9]+'
WHITESPACE = #'[,\ \t]+'
{% endhighlight %}

A final consideration is operator precedence of multiplication over addition and subtraction.
I leave this as an exercise for the interested reader ;)

#### Main function

Now we only need a main function to be able to use the calculator program.

{% highlight clojure %}
(ns clj-calculator.core
    (:require [instaparse.core :as insta])
    (:gen-class))

; ...

(defn -main
  [& _args]
  (loop [line (read-line)]
        (when line
          (println (calculate line))
          (recur (read-line))))
  (System/exit 0))
{% endhighlight %}

Now one can run the program as follows:

{% highlight bash %}
clj -M -m clj-calculator.core
{% endhighlight %}

To exit the calculator, simply press CTRL+D.

See [github.com/wedesoft/clj-calculator][6] for source code.

Enjoy!

[1]: https://clojure.org/
[2]: https://github.com/Engelberg/instaparse
[3]: https://github.com/marick/Midje/
[4]: https://github.com/wedesoft/midje-runner
[5]: https://github.com/miorimmax/midje-runner
[6]: https://github.com/wedesoft/clj-calculator
