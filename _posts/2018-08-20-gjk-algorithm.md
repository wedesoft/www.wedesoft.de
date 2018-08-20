---
layout: post
title: A divide-and-conquer implementation of the GJK algorithm
category: simulation
---

The [*Gilbert-Johnson-Keerthi* (GJK) algorithm][1] is a method for collision detection of convex polyhedra.
The method is used in multiple rigid body simulations for computer games.

Given:

* a set of points defining the convex polyhedron *A*
* a set of points defining the convex polyhedron *B*

The algorithm returns:

* the distance of the polyhedra
* two closest points
* the two simplices (convex subsets of up to 4 points in 3 dimensions) of *A* and *B* containing the two closest points

An n-dimensional simplex is the convex hull of n+1 points as shown in the figure below.

![n-dimensional simplices for different values of n](/pics/simplices.svg)

The algorithm makes use of the fact, that the distance between two sets *A* and *B* is the distance of the Minkowski difference to the origin:

{% latex usepackages=amsmath %}
$\displaystyle\mathop{\operatorname{min}}_{\vec{a}\in A, \vec{b}\in B}||\vec{a}-\vec{b}||=\mathop{\operatorname{min}}_{\vec{x}\in M(A, B)}||\vec{x}||$
{% endlatex %}

where

{% latex usepackages=txfonts %}
$M(A, B)\coloneqq\{\vec{a}-\vec{b}|\vec{a}\in A, \vec{b}\in B\}$
{% endlatex %}

The *GJK* algorithm iteratively updates a simplex until the closest point to the origin is found.

The algorithm iterates using support points.
Given a set *M* and a vector *d*, the support point is defined as the furthest point of *M* in direction *d*:

{% latex usepackages=amsmath,txfonts %}
$s(M,\vec{d})\coloneqq\displaystyle\mathop{\operatorname{argmax}}_{\vec{x}\in M}\vec{x}\cdot\vec{d}$
{% endlatex %}

The *GJK* algorithm detects the two closest points of *A* and *B* as follows:

1. Choose any point *m* in *M(A,B)* in the Minkowski difference of *A* and *B*.
1. Set the initial simplex *w<sub>0</sub>* to *w<sub>0</sub>={m}*.
1. Let *d* be the closest point of *w<sub>k</sub>* to the origin.
1. If *d s(w<sub>k</sub>, -d)>=d s(M,-d)* then return *d*.
1. Set *w'<sub>k+1</sub>=w<sub>k</sub>∪{s(M,-d)}*
1. Set *w<sub>k+1</sub>* to the smallest simplex in *w'<sub>k+1</sub>* still containing *s(M,-d)*.

Note that step 3 requires finding the closest point of the simplex *w<sub>k</sub>* to the origin.
Most implementations of the *GJK* algorithm seem to use the following approach:

* Check if the origin is contained in the 3D simplex.
* If not, check whether the origin is near one of the 4 surfaces of the simplex.
* If the origin is not in the Delaunay region of any surface, check whether the origin is near one of the 6 edges.
* If not in a Delaunay region of an edge, find the closest point of the 4 points.

A much more compact implementation can be obtained using a divide-and-conquer approach:

* Let *w<sub>k</sub>={w<sub>k0</sub>,w<sub>k1</sub>,...,w<sub>kn</sub>}*
* Solve the least squares equation system
{% latex usepackages=amsmath,txfonts %}
$\displaystyle\mathop{\operatorname{argmin}}_{\vec{t}}||\underbrace{\begin{pmatrix}w_{k1}-w_{k0} & w_{kn}-w_{k0}\end{pmatrix}}_{\eqqcolon\mathcal{H}}\vec{t}-\underbrace{w_{k0}}_{\eqqcolon b}||$
{% endlatex %}
* If all *t<sub>i</sub>>=0* and *t<sub>1</sub>+t<sub>2</sub>+...+t<sub>n</sub><=1* (or *n=0*), then *w<sub>k0</sub>+Ht* is the closest point.
* Otherwise take the closest point of all sub-simplices with *n-1* dimensions using the approach above (*i.e.* recursion).

Note that this approach will visit each edge up to two times, and each point up to three times.
The performance is not optimal, but it makes for a much more concise implementation.

The least squares solution is:

{% latex usepackages=amsmath %}
$\displaystyle\mathop{\operatorname{argmin}}_{\vec{t}}||\mathcal{H}\vec{t}-\vec{b}||=(\mathcal{H}^\top\mathcal{H})^{-1}\mathcal{H}^\top\vec{b}$
{% endlatex %}

Here follows an implementation in Scheme (GNU Guile).
By using pairs of points from *A* and *B* instead of the Minkowski difference, one can keep track of the information required to determine the pair of closest points of *A* and *B* (instead of the closest point of *M* to the origin).

{% highlight scheme %}
(use-modules (oop goops) (srfi srfi-1) (srfi srfi-26))

(define-method (- (a <list>))
  "Negate vector."
  (map - a))

(define-method (+ (a <list>) (b <list>))
  "Add two vectors."
  (map + a b))

(define-method (+ (a <list>) (b <real>))
  "Add vector and scalar."
  (map (cut + <> b) a))

(define-method (+ (a <real>) (b <list>))
  "Add vector and scalar."
  (map (cut + a <>) b))

(define-method (- (a <list>) (b <list>))
  "Subtract a vector from another."
  (map - a b))

(define-method (- (a <list>) (b <real>))
  "Subtract a vector from another."
  (map (cut - <> b) a))

(define-method (- (a <real>) (b <list>))
  "Subtract a vector from another."
  (map (cut - a <>) b))

(define-method (* (a <list>) (b <number>))
  "Multiply a vector with a scalar."
  (map (cut * <> b) a))

(define-method (* (a <number>) (b <list>))
  "Multiply a scalar with a vector."
  (map (cut * <> a) b))

(define (argop op fun lst)
  (let* [(vals  (map fun lst))
         (opval (apply op vals))]
    (list-ref (reverse lst) (1- (length (member opval vals))))))

(define (argmin fun lst) (argop min fun lst))

(define (argmax fun lst) (argop max fun lst))

(define (leave-one-out lst)
  (map (lambda (i) (append (take lst i) (drop lst (1+ i)))) (iota (length lst))))

(define (inner-product a b)
  "Compute inner product of two vectors."
  (reduce + 0 (map * a b)))

(define (norm v)
  "Return norm of a vector."
  (sqrt (inner-product v v)))

(define (transpose mat)
  "Transpose a matrix"
  (if (null? mat)
    '()
    (map (lambda (i) (map (cut list-ref <> i) mat)) (iota (length (car mat))))))

(define (dot mat vec)
  "Multiply a matrix with another matrix or a vector"
  (map (cut inner-product <> vec) mat))

(define (permutations lst)
  "Return all permutations of list LST. The permutations are ordered so that every alternate permutation is even."
  (if (zero? (length lst))
    '(())
    (concatenate
      (map
        (lambda (item index)
          (let [(remaining (delete item lst))
                (order     (if (even? index) identity reverse))]
            (map (cut cons item <>) (permutations (order remaining)))))
        lst
        (iota (length lst))))))

(define (determinant mat)
  "Compute determinant of a matrix"
  (let* [(n       (length mat))
         (indices (iota n))
         (perms   (permutations indices))]
    (reduce + 0
      (map
        (lambda (perm k)
          (* (reduce * 1 (map (lambda (j i) (list-ref (list-ref mat j) i))
                              indices perm))
             (if (even? k) 1 -1)))
         perms
         (iota (length perms))))))

(define (submatrix mat row column)
  "Return submatrix with specified ROW and COLUMN removed."
  (let [(rows    (delete row    (iota (length mat))))
        (columns (delete column (iota (length (car mat)))))]
    (map (lambda (j) (map (lambda (i) (list-ref (list-ref mat j) i)) columns)) rows)))

(define (inverse mat)
  "Compute inverse of matrix"
  (let [(det     (determinant mat))
        (indices (iota (length mat)))
        (sgn     (lambda (v j i) (if (eq? (even? j) (even? i)) v (- v))))]
    (map (lambda (j)
           (map (lambda (i) (sgn (/ (determinant (submatrix mat i j)) det) j i))
                indices))
         indices)))


(define (least-squares design-matrix observation)
  "Least-squares solver"
  (if (null? design-matrix)
    '()
    (let [(design-matrix-transposed (transpose design-matrix))]
      (dot (inverse (dot design-matrix-transposed design-matrix))
           (dot design-matrix-transposed observation)))))

(define (support-point direction points)
  "Get outermost point of POINTS in given DIRECTION."
  (argmax (cut inner-product direction <>) points))

(define (center-of-gravity points)
  "Compute average of given points"
  (* (reduce + #f points) (/ 1 (length points))))

(define (closest-simplex-points simplex-a simplex-b)
  "Determine closest point pair of two simplices"
  (let* [(observation   (- (car simplex-a) (car simplex-b)))
         (design-matrix (- observation (transpose (- (cdr simplex-a)
                                                     (cdr simplex-b)))))
         (factors       (least-squares design-matrix observation))]
      (if (and (every positive? factors) (< (reduce + 0 factors) 1))
        (cons (cons (fold + (car simplex-a)
                          (map * factors
                               (map (cut - <> (car simplex-a)) (cdr simplex-a))))
                    (fold + (car simplex-b)
                          (map * factors
                               (map (cut - <> (car simplex-b)) (cdr simplex-b)))))
              (cons simplex-a simplex-b))
        (argmin (lambda (result) (norm (- (caar result) (cdar result))))
                (map closest-simplex-points
                     (leave-one-out simplex-a)
                     (leave-one-out simplex-b))))))

(define (gjk-algorithm convex-a convex-b)
  "Get pair of closest points of two convex hulls each defined by a set of points"
  (let [(simplex-a '())
        (simplex-b '())
        (closest (cons (center-of-gravity convex-a) (center-of-gravity convex-b)))]
    (while #t
      (let* [(direction  (- (car closest) (cdr closest)))
             (candidates (cons (support-point (- direction) convex-a)
                               (support-point direction convex-b)))]
        (if (>= (+ (inner-product direction (- direction)) 1e-12)
                (inner-product (- (car candidates) (cdr candidates)) (- direction)))
          (break closest))
        (let [(result (closest-simplex-points (cons (car candidates) simplex-a)
                                              (cons (cdr candidates) simplex-b)))]
          (set! closest (car result))
          (set! simplex-a (cadr result))
          (set! simplex-b (cddr result)))))))
{% endhighlight %}

Here an example of two colliding cuboids simulated using this implementation is shown:

{% youtube "https://www.youtube.com/watch?v=cypRZzhfXnE" %}

Any feedback, comments, and suggestions are welcome.

Enjoy!

[1]: https://en.wikipedia.org/wiki/Gilbert%E2%80%93Johnson%E2%80%93Keerthi_distance_algorithm
