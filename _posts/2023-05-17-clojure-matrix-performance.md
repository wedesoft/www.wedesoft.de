---
layout: post
title: Clojure/Java Matrix Library Performance Comparison
category: software
image: /pics/matrix.jpg
---

This is a quick performance comparison of the [Clojure core.matrix library][1] and the [Efficient Java Matrix Library][2].
Because core.matrix uses the [VectorZ Java library][3] as a backend, direct calls to VectorZ were also included in the comparison.
Finally I added [fastmath][11] to the comparison after it was pointed out to me by the developer.
The [criterium 0.4.6][4] benchmark library was used to measure the performance of common matrix expressions.
Here are the results running it on an AMD Ryzen 7 4700U with a turbo speed of 4.1 GHz:

| op                                | core.matrix 0.63.0 | ejml-all 0.43 | vectorz-clj 0.48.0 | fastmath 2.2.1     |
|-----------------------------------|--------------------|---------------|--------------------|--------------------|
| make 4x4 matrix                   | 675 ns             | 135 ns        | 50.5 ns            | **13.1 ns**        |
| make 4D vector                    | 299 ns             | 47.6 ns       | 9.27 ns            | **3.67 ns**        |
| add 4D vectors                    | 13.5 ns            | 18.2 ns       | 9.02 ns            | **4.29 ns**        |
| inverse matrix                    | 439 ns             | 81.4 ns       | 440 ns             | **43.6 ns**        |
| elementwise matrix multiplication | 64.9 ns            | 29.0 ns       | 29.1 ns            | **13.7 ns**        |
| matrix multiplication             | 102 ns             | 74.7 ns       | 100 ns             | **22.4 ns**        |
| matrix-vector multiplication      | 20.9 ns            | 31.2 ns       | 19.1 ns            | **6.46 ns**        |
| vector dot product                | 6.56 ns            | 6.90 ns       | **4.46 ns**        | 6.36 ns            |
| vector norm                       | 10.1 ns            | 11.4 ns       | not supported?     | **3.74 ns**        |
| matrix determinant                | 170 ns             | **7.35 ns**   | 166 ns             | 7.67 ns            |
| matrix element access             | 4.14 ns            | 3.35 ns       | **3.26 ns**        | 3.53 ns<sup>1</sup>|
| get raw data array                | 12.0 ns            | **3.00 ns**   | 11.9 ns            | 13.2 ns<sup>1</sup>|

<sup>1</sup><small>requires fastmath 2.2.2-SNAPSHOT or later</small>

See [matperf.clj][12] for source code of benchmark script.

Comparing EJML with a mix of core.matrix and direct calls to vectorz:
* EJML has support for both single and double precision floating point numbers
* it uses single column matrices to represent vectors leading to slower matrix-vector multiplication
* it has a fast 4x4 matrix inverse
* it does not come with a Clojure wrapper
* it offers fast access to raw data

Comparing EJML with fastmath:
* EJML has support for matrices larger than 4x4
* EJML gives you access to the matrix as a flat floating point array (fastmath will add support in the future)
* EJML is mostly slower

The implementations of the libraries are all quite impressive with custom optimisations for small matrices and vectors.
Note that I didn't include [Neanderthal][5] in the comparison because it is more suitable for large matrices.

I hope you find this comparison useful.

**Update:**

The large performance difference for matrix inversion is probably because EJML has custom 4x4 matrix classes while VectorZ stops at 3x3.
Here is a performance comparison of matrix inverse for 3x3, 4x4, and 5x5 matrices:

| op                 | core.matrix 0.63.0 | ejml-all 0.43 | vectorz-clj 0.48.0 | fastmath 2.2.1 |
|--------------------|--------------------|---------------|--------------------|----------------|
| 3x3 matrix inverse | 13.0 ns            | 48.3 ns       | 12.2 ns            | **10.8 ns**    |
| 4x4 matrix inverse | 471 ns             | 98.3 ns       | 465 ns             | **50.3 ns**    |
| 5x5 matrix inverse | 669 ns             | **172 ns**    | 666 ns             | not supported  |

**Further updates:**

* posted it on [Reddit][6]
* [weavejester/euclidean][7] is a small library implementing small square matrices and vectors.
* [play-cljc comes with a matrix library as well][8].
* there is also [ojalgo][9] which according to the [Java Matrix Benchmark][10] is the fasted pure Java linear algebra library.
* [fastmath][11] is a very fast Clojure library which provides double-precision matrix operations up to dimension 4.

[1]: https://mikera.github.io/core.matrix/
[2]: http://ejml.org/
[3]: https://github.com/mikera/vectorz
[4]: https://github.com/mikera/vectorz
[5]: https://neanderthal.uncomplicate.org/
[6]: https://www.reddit.com/r/Clojure/comments/13khc0h/clojurejava_matrix_library_performance_comparison/
[7]: https://github.com/weavejester/euclidean
[8]: https://github.com/oakes/play-cljc/blob/master/src/play_cljc/math.cljc
[9]: https://www.ojalgo.org/
[10]: https://lessthanoptimal.github.io/Java-Matrix-Benchmark/
[11]: https://github.com/generateme/fastmath
[12]: https://gist.github.com/wedesoft/8d1f8646649037904f16915f4639228e
