---
layout: page
title: Graph colouring
image: pics/graphc.png
---

Currently I am looking into [register allocation using graph colouring][1]
(also see [Chaitin's paper][x] on the subject). The [graph colouring][8] problem also occurs when
trying to colour countries in a political world map where adjacent countries must have different colours.
I thought it would be interesting to try a minimal implementation in Scheme.
Feel free to suggest improvements in the comment section below <img src="/pics/bounce.gif" width="19" alt=""/>.

A (undirected) [graph][2] is usually represented by a list of nodes (vertices) and a list of edges.
If there are no insular nodes, a list of edges is sufficient.
In Scheme (here [GNU Guile][3] implementation) one can do this as follows.

{% highlight scheme %}
'((b . a) (a . c) (d . c))
{% endhighlight %}

The graph can be visualised on the command line using [Graphviz][4] and [ImageMagick][5] as follows.

<span class="right"><img src="/pics/graph.png" width="135" alt=""/></span>

{% highlight shell %}
echo 'graph test { b -- a; a -- c; d -- c; }' | dot -Tpng | display -
{% endhighlight %}

The following helper function *graphviz* uses a system call to display a graph from within the REPL.

{% highlight scheme %}
(define (dot graph colors)
  (apply string-append
         (append (list "graph g {")
                 (map (lambda (color) (format #f " ~a [style=filled, fillcolor=~a];" (car color) (cdr color))) colors)
                 (map (lambda (edge) (format #f " ~a -- ~a;" (car edge) (cdr edge))) graph)
                 (list " }"))))
(define (graphviz graph colors) (system (format #f "echo '~a' | dot -Tpng | display -" (dot graph colors))))
(graphviz '((b . a) (a . c) (d . c)) '())
{% endhighlight %}

One can get the nodes of the graph by extracting all elements and
suppressing any duplicates. The definition of [delete-duplicates][6] is part of [SRFI-1][7]
(*Scheme Request for Implementation 1*).

{% highlight scheme %}
(use-modules (srfi srfi-1))
(define (nodes graph) (delete-duplicates (append (map car graph) (map cdr graph))))
(nodes '((b . a) (a . c) (d . c)))
; (b a d c)
{% endhighlight %}

The adjacency list of a node is simply the list of nodes of the sub-graph obtained by filtering
for edges connecting to this node.

{% highlight scheme %}
(use-modules (ice-9 curried-definitions))
(define ((has-node? node) edge) (or (eq? (car edge) node) (eq? (cdr edge) node)))
(define (adjacent graph node) (nodes (filter (has-node? node) graph)))
(adjacent '((b . a) (a . c) (d . c)) 'c)
; (a d c)
{% endhighlight %}

[Chaitin's graph coloring algorithm][x] works by successively removing nodes with a low adjacency count from the graph. Removing a node from our graph can be done as follows.

{% highlight scheme %}
(define (remove-node graph node) (filter (compose not (has-node? node)) graph))
(remove-node '((b . a) (a . c) (d . c)) 'c)
; ((b . a))
{% endhighlight %}

Using the *argument of the minimum* one can determine the node with lowest adjacency count.

{% highlight scheme %}
(define (argmin fun lst)
  (let* [(vals   (map fun lst))
         (minval (apply min vals))]
    (list-ref lst (- (length lst) (length (member minval vals))))))
{% endhighlight %}

Now one can recursively remove the node with lowest adjacency count and then assign colours starting with the last node and working backwards.
If an adjacent node has a colour already, another colour must be used.

<span class="right"><img src="/pics/graphc.png" width="135" alt=""/></span>

{% highlight scheme %}
(use-modules (srfi srfi-26))
(define (assign-colors graph nodes colors)
  (if (null? nodes) '()
    (let* [(target    (argmin (compose length (cut adjacent graph <>)) nodes))
           (coloring  (assign-colors (remove-node graph target) (delete target nodes) colors))
           (blocked   (map (cut assq-ref coloring <>) (adjacent graph target)))
           (available (lset-difference eq? colors blocked))]
      (cons (cons target (car available)) coloring))))
(define (coloring graph colors) (assign-colors graph (nodes graph) colors))
(coloring '((b . a) (a . c) (d . c)) '(red green blue))
; ((b . red) (a . green) (d . green) (c . red))
(let [(graph '((b . a) (a . c) (d . c)))] (graphviz graph (coloring graph '(red green blue))))
{% endhighlight %}

And here is an example of coloring a graph with a few more nodes.

<span class="right"><img src="/pics/process.png" width="135" alt=""/></span>

{% highlight scheme %}
(let [(graph '((run . intr)
               (intr . runbl)
               (runbl . run)
               (run . kernel)
               (kernel . zombie)
               (kernel . sleep)
               (kernel . runmem)
               (sleep . swap)
               (swap . runswap)
               (runswap . new)
               (runswap . runmem)
               (new . runmem)
               (sleep . runmem)))]
  (graphviz graph (coloring graph '(red green blue yellow))))
{% endhighlight %}


[1]: http://www.lighterra.com/papers/graphcoloring/
[x]: http://www.inf.usi.ch/faculty/nystrom/teaching/compilers/fa12/doc/p66-chaitin.pdf
[2]: http://en.wikipedia.org/wiki/Graph_%28abstract_data_type%29
[3]: https://www.gnu.org/software/guile/
[4]: http://www.graphviz.org/
[5]: http://www.imagemagick.org/
[6]: https://www.gnu.org/software/guile/manual/html_node/SRFI_002d1-Deleting.html
[7]: https://www.gnu.org/software/guile/manual/html_node/SRFI_002d1.html
[8]: http://en.wikipedia.org/wiki/Graph_coloring
