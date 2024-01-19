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
The Clojure version was 1.11.1 and OpenJDK runtime version was 17.0.6.
Here are the results running it on an AMD Ryzen 7 4700U with a turbo speed of 4.1 GHz:

| op                                          | core. matrix 0.63.0 | ejml-all 0.43    | vectorz- clj 0.48.0 | fastmath 2.2.1     |
|---------------------------------------------|---------------------|------------------|---------------------|--------------------|
| make 4x4 matrix                             | 675&nbsp;ns         | 135&nbsp;ns      | 50.5&nbsp;ns        | **13.1&nbsp;ns**        |
| make 4D vector                              | 299&nbsp;ns         | 47.6&nbsp;ns     | 9.27&nbsp;ns        | **3.67&nbsp;ns**        |
| add 4D vectors                              | 13.5&nbsp;ns        | 18.2&nbsp;ns     | 9.02&nbsp;ns        | **4.29&nbsp;ns**        |
| inverse matrix                              | 439&nbsp;ns         | 81.4&nbsp;ns     | 440&nbsp;ns         | **43.6&nbsp;ns**        |
| element&shy;wise matrix multi&shy;plication | 64.9&nbsp;ns        | 29.0&nbsp;ns     | 29.1&nbsp;ns        | **13.7&nbsp;ns**        |
| matrix multi&shy; plication                 | 102&nbsp;ns         | 74.7&nbsp;ns     | 100&nbsp;ns         | **22.4&nbsp;ns**        |
| matrix-vector multi&shy;plication           | 20.9&nbsp;ns        | 31.2&nbsp;ns     | 19.1&nbsp;ns        | **6.46&nbsp;ns**        |
| vector dot product                          | 6.56&nbsp;ns        | 6.90&nbsp;ns     | **4.46&nbsp;ns**    | 6.36&nbsp;ns            |
| vector norm                                 | 10.1&nbsp;ns        | 11.4&nbsp;ns     | no support?         | **3.74&nbsp;ns**        |
| matrix deter&shy;minant                    | 170&nbsp;ns         | **7.35&nbsp;ns** | 166&nbsp;ns         | 7.67&nbsp;ns            |
| matrix element access                       | 4.14&nbsp;ns        | 3.35&nbsp;ns     | **3.26&nbsp;ns**    | 3.53&nbsp;ns<sup>1</sup>|
| get raw data array                          | 12.0&nbsp;ns        | **3.00&nbsp;ns** | 11.9&nbsp;ns        | 13.2&nbsp;ns<sup>1</sup>|

<sup>1</sup><small>requires fastmath 2.2.2-SNAPSHOT or later</small>

See [matperf.clj][12] for source code of benchmark script.

Comparing EJML with a mix of core.matrix and direct calls to vectorz:
* EJML has support for both single and double precision floating point numbers
* it uses single column matrices to represent vectors leading to slower matrix-vector multiplication
* it has a fast 4x4 matrix inverse
* it does not come with a Clojure wrapper
* it offers fast access to raw data
* it does not support multi-dimensional arrays

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

| op                 | core. matrix 0.63.0 | ejml-all 0.43   | vectorz- clj 0.48.0 | fastmath 2.2.1   |
|--------------------|---------------------|-----------------|---------------------|------------------|
| 3x3 matrix inverse | 13.0&nbsp;ns        | 48.3&nbsp;ns    | 12.2&nbsp;ns        | **10.8&nbsp;ns** |
| 4x4 matrix inverse | 471&nbsp;ns         | 98.3&nbsp;ns    | 465&nbsp;ns         | **50.3&nbsp;ns** |
| 5x5 matrix inverse | 669&nbsp;ns         | **172&nbsp;ns** | 666&nbsp;ns         | not supported    |

**Further updates:**

* posted it on [Reddit][6]
* [weavejester/euclidean][7] is a small library implementing small square matrices and vectors.
* [play-cljc comes with a matrix library as well][8].
* there is also [ojalgo][9] which according to the [Java Matrix Benchmark][10] is the fasted pure Java linear algebra library.
* [fastmath][11] is a very fast Clojure library which provides double-precision matrix operations up to dimension 4.
* 18/01/2024: fastmath version 2.3.0 was released

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
