---
layout: post
title: Developing a Space Flight Simulator in Clojure
category: software
image: /pics/sfsim.jpg
---

{% youtube "https://www.youtube.com/watch?v=38FGT7SWVh0" %}

In 2017 discovered the free of charge [Orbiter 2016][16] space flight simulator which was proprietary at the time and it eventually inspired me to develop a space flight simulator myself.
I prototyped some rigid body physics in C and later in [GNU Guile][17] and also prototyped loading and rendering of Wavefront OBJ files.
I used GNU Guile (a Scheme implementation) because it has a good native interface and of course it has hygienic macros.
Eventually I got interested in Clojure because unlike GNU Guile it has multi-methods as well as fast hash maps and vectors.
I finally decided to develop the game for real in Clojure.
I have been developing a space flight simulator in Clojure for almost 5 years now.
While using Clojure I have come to appreciate the immutable values and safe parallelism using atoms, agents, and refs.

In the beginning I decided to work on the hard parts first which for me were 3D rendering of a planet, an atmosphere, shadows, and volumetric clouds.
I read the [OpenGL Superbible][19] to get an understanding on what functionality OpenGL implementations provide.
When Orbiter was eventually open sourced and released unter MIT license [here][18], I inspected the source code and discovered that about 90% of the code is graphics-related.


* Bruneton paper using atmospheric lookup tables, render background quad (essentially ray tracing)
* generate blue noise, worley noise, perlin noise (spit-floats) use pfor and generate flat vector with 2D or 3D noise data
* Use templating language to make flexible shaders, nested lists of shaders are used to specify dependencies and build shader programs
* astronomy from Skyfield
* converting NASA Bluemarble data and elevation data into multi resolution map and then cube map of quad trees, data gets loaded up to required level of detail in a background thread and then loaded into OpenGL textures in the main thread, tessellation information for neighbouring quads gets updated in the background as well (using a future)
* quaternion implementation
* aerodynamics using fastmath splines and fastmath coordinate transformations
* Unit tested shader functions using shader mocks and templating (e.g. noise-octaves)
* deep opacity maps, render clouds in front of background quad, planet and space craft
* render fonts to a texture, Nuklear GUI immediate mode commands fill a render buffer with chunks of triangles which then get rendered using a generic shader
* input code uses multimethods to dispatch events to methods of an input handler (protocol), a hash map is used to map keys, buttons, and axes to keywords which are then used to dispatch to code for making state changes (atom referencing a hash map)
* Jolt bindings using Coffi
* Use Assimp to read glTF model with animations into a nested data structure
* physics uses multi methods to map from source to target coordinate system
* Steam does not like insane number of files, so map tiles are stored in tar files and random accessed using an lru cache of opened tar files

* [Clojure][1] the programming languagte
* [LWJGL][15] provides Java wrappers for various libraries
  * lwjgl-opengl for 3D graphics
  * lwjgl-glfw for windowing and input devices
  * lwjgl-nuklear for graphical user interfaces
  * lwjgl-stb for image I/O and using truetype fonts
  * lwjgl-assimp to load glTF 3D models with animation data
* [Jolt Physics][14] to simulate wheeled vehicles and collisions with meshes
* [Fastmath][9] for fast matrix and vector math
* [Comb][6] for templating shader code
* [Instaparse][10] to parse NASA Planetary Constant Kernel (PCK) files
* [Gloss][11] to parse NASA Double Precision Array Files (DAF)
* [Coffi][13] as a foreign function interface
* [core.memoize][5] for least recently used caching of function results
* [Apache Commons Compress][12] to read map tiles from tar files
* [Malli][2] to add schemas to functions
* [Immuconf][3] to load the configuration file
* [Progrock][7] a progress bar for long running builds
* [Claypoole][8] to implement parallel for loops
* [slf4j-timbre][4] Java logging implementation for Clojure


[1]: https://clojure.org/
[2]: https://github.com/metosin/malli
[3]: https://github.com/levand/immuconf
[4]: https://github.com/fzakaria/slf4j-timbre
[5]: https://github.com/clojure/core.memoize
[6]: https://github.com/weavejester/comb
[7]: https://github.com/weavejester/progrock
[8]: https://github.com/clj-commons/claypoole
[9]: https://generateme.github.io/fastmath/
[10]: https://github.com/Engelberg/instaparse
[11]: https://github.com/clj-commons/gloss
[12]: https://commons.apache.org/proper/commons-compress/
[13]: https://github.com/IGJoshua/coffi
[14]: https://github.com/jrouwe/JoltPhysics
[15]: https://www.lwjgl.org/
[16]: http://orbit.medphys.ucl.ac.uk/
[17]: https://www.gnu.org/software/guile/
[18]: https://github.com/orbitersim/orbiter
[19]: https://www.informit.com/store/opengl-superbible-comprehensive-tutorial-and-reference-9780672337475
