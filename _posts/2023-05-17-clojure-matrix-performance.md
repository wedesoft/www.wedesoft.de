---
layout: post
title: Clojure/Java Matrix Library Performance Comparison
category: software
image: /pics/matrix.jpg
---

This is a quick performance comparison of the [Clojure core.matrix library][1] and the [Efficient Java Matrix Library][2].
Because core.matrix uses the [VectorZ Java library][3] as a backend, direct calls to VectorZ were also included in the comparison.
The [criterium 0.4.6][4] benchmark library was used to measure the performance of common matrix expressions.
Here are the results running it on an AMD Ryzen 7 4700U with a turbo speed of 4.1 GHz:

| op                                | core.matrix 0.63.0 | ejml 0.43     | vectorz-clj 0.48.0 |
|-----------------------------------|--------------------|---------------|--------------------|
| make 4x4 matrix                   | 677 ns             | 133 ns        | **53.4 ns**        |
| make 4D vector                    | 259 ns             | 47.2 ns       | **8.53 ns**        |
| add 4D vectors                    | **12.8 ns**        | 17.6 ns       | 18.0 ns            |
| elementwise matrix multiplication | 64.6 ns            | 27.9 ns       | **26.9 ns**        |
| matrix multiplication             | 116 ns             | **75.7 ns**   | 112 ns             |
| matrix-vector multiplication      | 21.4 ns            | 31.3 ns       | **19.3 ns**        |
| vector dot product                | 6.60 ns            | 6.94 ns       | **4.42 ns**        |
| inverse matrix                    | 461 ns             | **81.4 ns**   | 456 ns             |
| matrix determinant                | 153 ns             | **6.87 ns**   | 150 ns             |
| matrix element access             | **2.89 ns**        | 3.09 ns       | 3.08 ns            |
| get raw data array                | 11.8 ns            | **0.0383 ns** | 11.7 ns            |

See [here][12] for source code of benchmark script.

Comparing EJML with a mix of core.matrix and direct calls to vectorz:
* EJML has support for both single and double precision floating point numbers
* it uses single column matrices to represent vectors leading to slower matrix-vector multiplication
* it has a fast 4x4 matrix inverse
* it does not come with a Clojure wrapper
* it offers fast access to raw data

The implementations of the libraries are all quite impressive with custom optimisations for small matrices and vectors.
Note that I didn't include [Neanderthal][5] in the comparison because it is more suitable for large matrices.

I hope you find this comparison useful.

**Update:**

The large performance difference for matrix inversion is probably because EJML has custom 4x4 matrix classes while VectorZ stops at 3x3.
Here is a performance comparison of matrix inverse for 3x3, 4x4, and 5x5 matrices:

| op                 | core.matrix 0.63.0 | ejml 0.43     | vectorz-clj 0.48.0 |
|--------------------|--------------------|---------------|--------------------|
| 3x3 matrix inverse | **12.9 ns**        | 47.9 ns       | 20.5 ns            |
| 4x4 matrix inverse | 449 ns             | **95.9 ns**   | 451 ns             |
| 5x5 matrix inverse | 651 ns             | **197 ns**    | 637 ns             |

**Further updates:**

* posted it on [Reddit][6]
* [weavejester/euclidean][7] is a small library implementing small square matrices and vectors.
* [play-cljc comes with a matrix library as well][8].
* there is also [ojalgo][9] which according to the [Java Matrix Benchmark][10] is the fasted pure Java linear algebra library.
* [fastmath][11] is a much faster Clojure library which provides matrix operations up to dimension 4.

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
