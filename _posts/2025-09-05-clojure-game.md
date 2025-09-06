---
layout: post
title: Developing a Space Flight Simulator in Clojure
category: software
image: /pics/sfsim.jpg
---

{% youtube "https://www.youtube.com/watch?v=38FGT7SWVh0" %}

In 2017 I discovered the free of charge [Orbiter 2016][16] space flight simulator which was proprietary at the time and it inspired me to develop a space flight simulator myself.
I prototyped some rigid body physics in C and later in [GNU Guile][17] and also prototyped loading and rendering of Wavefront OBJ files.
I used GNU Guile (a Scheme implementation) because it has a good native interface and of course it has hygienic macros.
Eventually I got interested in Clojure because unlike GNU Guile it has multi-methods as well as fast hash maps and vectors.
I finally decided to develop the game for real in Clojure.
I have been developing a space flight simulator in Clojure for almost 5 years now.
While using Clojure I have come to appreciate the immutable values and safe parallelism using atoms, agents, and refs.

In the beginning I decided to work on the hard parts first, which for me were 3D rendering of a planet, an atmosphere, shadows, and volumetric clouds.
I read the [OpenGL Superbible][19] to get an understanding on what functionality OpenGL provides.
When Orbiter was eventually open sourced and released unter MIT license [here][18], I inspected the source code and discovered that about 90% of the code is graphics-related.
So starting with the graphics problems was not a bad decision.

## Software dependencies

The following software is used for development.
The software libraries run on both GNU/Linux and Microsoft Windows.

* [Clojure][1] the programming language
* [LWJGL][15] provides Java wrappers for various libraries
  * lwjgl-opengl for 3D graphics
  * lwjgl-glfw for windowing and input devices
  * lwjgl-nuklear for graphical user interfaces
  * lwjgl-stb for image I/O and using truetype fonts
  * lwjgl-assimp to load glTF 3D models with animation data
* [Jolt Physics][14] to simulate wheeled vehicles and collisions with meshes
* [Fastmath][9] for fast matrix and vector math as well as spline interpolation
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
* [tools.build][31] to build the project
* [clj-async-profiler][24] Clojure profiler creating flame graphs
* [slf4j-timbre][4] Java logging implementation for Clojure

The *deps.edn* file contains operating system dependent LWJGL bindings.
For example on GNU/Linux the *deps.edn* file contains the following:

{% highlight clojure %}
{:deps {; ...
        org.lwjgl/lwjgl {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl$natives-linux {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-opengl {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-opengl$natives-linux {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-glfw {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-glfw$natives-linux {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-nuklear {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-nuklear$natives-linux {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-stb {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-stb$natives-linux {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-assimp {:mvn/version "3.3.6"}
        org.lwjgl/lwjgl-assimp$natives-linux {:mvn/version "3.3.6"}}
        ; ...
        }
{% endhighlight %}

In order to manage the different dependencies for Microsoft Windows, a separate Git branch is maintained.

## Atmosphere rendering

{% youtube "https://www.youtube.com/watch?v=q9aWd_14qhA" %}

For the atmosphere, [Bruneton's precomputed atmospheric scattering][20] was used.
The implementation uses a 2D transmittance table, a 2D surface scattering table, a 4D Rayleigh scattering, and a 4D Mie scattering table.
The tables are computed using several iterations of numerical integration.
Higher order functions for integration over a sphere and over a line segment were implemented in Clojure.
Integration over a ray in 3D space (using fastmath vectors) was implemented as follows for example:

{% highlight clojure %}
(defn integral-ray
  "Integrate given function over a ray in 3D space"
  {:malli/schema [:=> [:cat ray N :double [:=> [:cat [:vector :double]] :some]] :some]}
  [{::keys [origin direction]} steps distance fun]
  (let [stepsize      (/ distance steps)
        samples       (mapv #(* (+ 0.5 %) stepsize) (range steps))
        interpolate   (fn interpolate [s] (add origin (mult direction s)))
        direction-len (mag direction)]
    (reduce add (mapv #(-> % interpolate fun (mult (* stepsize direction-len))) samples))))
{% endhighlight %}

Precomputing the atmospheric tables takes several hours even though [pmap][23] was used.
When sampling the multi-dimensional functions, *pmap* was used as a top-level loop and *map* was used for interior loops.
Using [java.nio.ByteBuffer][21] the floating point values were converted to a byte array and then written to disk using a [clojure.java.io/output-stream][22]:

{% highlight clojure %}
(defn floats->bytes
  "Convert float array to byte buffer"
  [^floats float-data]
  (let [n           (count float-data)
        byte-buffer (.order (ByteBuffer/allocate (* n 4)) ByteOrder/LITTLE_ENDIAN)]
    (.put (.asFloatBuffer byte-buffer) float-data)
    (.array byte-buffer)))

(defn spit-bytes
  "Write bytes to a file"
  {:malli/schema [:=> [:cat non-empty-string bytes?] :nil]}
  [^String file-name ^bytes byte-data]
  (with-open [out (io/output-stream file-name)]
    (.write out byte-data)))

(defn spit-floats
  "Write floating point numbers to a file"
  {:malli/schema [:=> [:cat non-empty-string seqable?] :nil]}
  [^String file-name ^floats float-data]
  (spit-bytes file-name (floats->bytes float-data)))
{% endhighlight %}

When launching the game, the lookup tables get loaded and copied into OpenGL textures.
Shader functions are used to lookup and interpolate values from the tables.
When rendering the planet surface or the space craft, the atmosphere essentially gets superimposed using ray tracing.
After rendering the planet, a background quad is rendered to display the remaining part of the atmosphere above the horizon.

## Templating OpenGL shaders

It is possible to make programming with OpenGL shaders more flexible by using a templating library such as *Comb*.
The following shader defines multiple octaves of noise on a base noise function:

{% highlight glsl %}
#version 410 core

float <%= base-function %>(vec3 idx);

float <%= method-name %>(vec3 idx)
{
  float result = 0.0;
<% (doseq [multiplier octaves] %>
  result += <%= multiplier %> * <%= base-function %>(idx);
  idx *= 2;
<% ) %>
  return result;
}
{% endhighlight %}

One can then for example define the function *fbm\_noise* using octaves of the base function *noise* as follows:

{% highlight clojure %}
(def noise-octaves
  "Shader function to sum octaves of noise"
  (template/fn [method-name base-function octaves] (slurp "resources/shaders/core/noise-octaves.glsl")))

; ...

(def fbm-noise-shader (noise-octaves "fbm_noise" "noise" [0.57 0.28 0.15]))
{% endhighlight %}

## Planet rendering

{% youtube "https://www.youtube.com/watch?v=Ce3oWQflYOY" %}

To render the planet, [NASA Bluemarble][25] data, [NASA Blackmarble][26] data, and [NASA Elevation][27] data was used.
The images were converted to a multi resolution pyramid of map tiles.
The following functions were implemented for color map tiles and for elevation tiles:

* a function to load and cache map tiles of given 2D tile index and level of detail
* a function to extract a pixel from a map tile
* a function to extract the pixel for a specific longitude and latitude

The functions for extracting a pixel for given longitude and latitude then were used to generate a cube map with a quad tree of tiles for each face.
For each tile, the following files were generated:

* A daytime texture
* A night time texture
* An image of 3D vectors defining a surface mesh
* A water mask
* A normal map

Altogether 655350 files were generated.
Because the Steam ContentBuilder does not support a large number of files, each row of tile data was aggregated into a tar file.
The *Apache Commons Compress* library allows you to open a tar file to get a list of entries and then perform random access on the contents of the tar file.
A Clojure LRU cache was used to maintain a cache of open tar files for improved performance.

At run time, a future is created, which returns an updated tile tree, a list of tiles to drop, and a path list of the tiles to load into OpenGL.
When the future is realized, the main thread deletes the OpenGL textures from the drop list, and then uses the path list to get the new loaded images from the tile tree, load them into OpenGL textures, and create an updated tile tree with the new OpenGL textures added.
The following functions to manipulate quad trees were implemented to realize this:

{% highlight clojure %}
(defn quadtree-add
  "Add tiles to quad tree"
  {:malli/schema [:=> [:cat [:maybe :map] [:sequential [:vector :keyword]] [:sequential :map]] [:maybe :map]]}
  [tree paths tiles]
  (reduce (fn add-title-to-quadtree [tree [path tile]] (assoc-in tree path tile)) tree (mapv vector paths tiles)))

(defn quadtree-extract
  "Extract a list of tiles from quad tree"
  {:malli/schema [:=> [:cat [:maybe :map] [:sequential [:vector :keyword]]] [:vector :map]]}
  [tree paths]
  (mapv (partial get-in tree) paths))

(defn quadtree-drop
  "Drop tiles specified by path list from quad tree"
  {:malli/schema [:=> [:cat [:maybe :map] [:sequential [:vector :keyword]]] [:maybe :map]]}
  [tree paths]
  (reduce dissoc-in tree paths))

(defn quadtree-update
  "Update tiles with specified paths using a function with optional arguments from lists"
  {:malli/schema [:=> [:cat [:maybe :map] [:sequential [:vector :keyword]] fn? [:* :any]] [:maybe :map]]}
  [tree paths fun & arglists]
  (reduce (fn update-tile-in-quadtree
            [tree [path & args]]
            (apply update-in tree path fun args)) tree (apply map list paths arglists)))
{% endhighlight %}

## Other topics

### Solar system

The astronomy code for getting the position and orientation of planets was implemented according to the [Skyfield][28] Python library.
The Python library in turn is based on the [SPICE][29] toolkit of the NASA JPL.
The JPL basically provides sequences of [Chebyshev polynomials][30] to interpolate positions of Moon and planets as well as the orientation of the Moon as binary files.
Reference coordinate systems and orientations of other bodies are provided in text files which consist of human and machine readable sections.
The binary files were parsed using *Gloss* (see [Wiki][32] for some examples) and the text files using *Instaparse*.

### Jolt bindings

The required Jolt functions for wheeled vehicle dynamics and collisions with meshes were wrapped in C functions and compiled into a shared library.
The *Coffi* Clojure library (which is a wrapper for Java's new Foreign Function & Memory API) was used to make the C functions and data types usable in Clojure.

For example the following code implements a call to the C function *add\_force*:

{% highlight clojure %}
(defcfn add-force
  "Apply a force in the next physics update"
  add_force [::mem/int ::vec3] ::mem/void)
{% endhighlight %}

Here *::vec3* refers to a custom composite type defined using basic types.
The memory layout, serialisation, and deserialisation for *::vec3* are defined as follows:

{% highlight clojure %}
(def vec3-struct
  [::mem/struct
   [[:x ::mem/double]
    [:y ::mem/double]
    [:z ::mem/double]]])


(defmethod mem/c-layout ::vec3
  [_vec3]
  (mem/c-layout vec3-struct))


(defmethod mem/serialize-into ::vec3
  [obj _vec3 segment arena]
  (mem/serialize-into {:x (obj 0) :y (obj 1) :z (obj 2)} vec3-struct segment arena))


(defmethod mem/deserialize-from ::vec3
  [segment _vec3]
  (let [result (mem/deserialize-from segment vec3-struct)]
    (vec3 (:x result) (:y result) (:z result))))
{% endhighlight %}

### Performance

The *clj-async-profiler* was used to create flame graphs visualising the performance of the game.
In order to get reflection warnings for Java calls without sufficient type declarations, *\*warn-on-reflection\** was set to *true*.
{% highlight clojure %}
(set! *warn-on-reflection* true)
{% endhighlight %}

Furthermore to discover missing declarations of numerical types, *\*unchecked-math\** was set to *:warn-on-boxed*.
{% highlight clojure %}
(set! *unchecked-math* :warn-on-boxed)
{% endhighlight %}

To reduce garbage collector pauses, the ZGC low-latency garbage collector for the JVM was used.
The following section in *deps.edn* ensures that the ZGC garbage collector is used when running the project with *clj -M:run*:

{% highlight clojure %}
{:deps {; ...
        }
 :aliases {:run {:jvm-opts ["-Xms2g" "-Xmx4g" "--enable-native-access=ALL-UNNAMED" "-XX:+UseZGC"
                            "--sun-misc-unsafe-memory-access=allow"]
                 :main-opts ["-m" "sfsim.core"]}}}
{% endhighlight %}

The option to use ZGC is also specified in the Packr JSON file used to deploy the application.

### Building the project

In order to build the map tiles, atmospheric lookup tables, and other data files using *tools.build*, the project source code was made available in the *build.clj* file using a *:local/root* dependency:
{% highlight clojure %}
{:deps {; ...
        }
 :aliases {; ...
           :build {:deps {io.github.clojure/tools.build {:mvn/version "0.10.10"}
                          sfsim/sfsim {:local/root "."}}
                   :ns-default build
                   :exec-fn all
                   :jvm-opts ["-Xms2g" "-Xmx4g" "--sun-misc-unsafe-memory-access=allow"]}}}
{% endhighlight %}

Various targets were defined to build the different components of the project.
For example the atmospheric lookup tables can be build by specifying *clj -T:build atmosphere-lut* on the command line.

The following section in the *build.clj* file was added to allow creating an "Uberjar" JAR file with all dependencies by specifying *clj -T:build uber* on the command-line.

{% highlight clojure %}
(defn uber [_]
  (b/copy-dir {:src-dirs ["src/clj"]
               :target-dir class-dir})
  (b/compile-clj {:basis basis
                  :src-dirs ["src/clj"]
                  :class-dir class-dir})
  (b/uber {:class-dir class-dir
           :uber-file "target/sfsim.jar"
           :basis basis
           :main 'sfsim.core}))
{% endhighlight %}

To create a Linux executable with Packr, one can then run *java -jar packr-all-4.0.0.jar scripts/packr-config-linux.json* where the JSON file has the following content:

{% highlight json %}
{
  "platform": "linux64",
  "jdk": "/usr/lib/jvm/jdk-24.0.2-oracle-x64",
  "executable": "sfsim",
  "classpath": ["target/sfsim.jar"],
  "mainclass": "sfsim.core",
  "resources": ["LICENSE", "libjolt.so", "venturestar.glb", "resources"],
  "vmargs": ["Xms2g", "Xmx4g", "XX:+UseZGC"],
  "output": "out-linux"
}
{% endhighlight %}

In order to distribute the game on Steam, three depots were created:

* a data depot with the operating system independent data files
* a Linux depot with the Linux executable and Uberjar including LWJGL's Linux native bindings
* and a Windows depot with the Windows executable and an Uberjar including LWJGL's Windows native bindings

When updating a depot, the Steam ContentBuilder command line tool creates and uploads a patch in order to preserve storage space and bandwidth.

## Future work

Although the hard parts are mostly done, there are still several things to do:

* control surfaces and thruster graphics
* launchpad and runway graphics
* sound effects
* a 3D cockpit
* the Moon
* a space station

It would also be interesting to make the game modable in a safe way (maybe evaluating Clojure files in a sandboxed environment?).

## Conclusion

{% youtube "https://www.youtube.com/watch?v=1PqmVLUt5_g" %}

You can find the [source code on Github](https://github.com/wedesoft/sfsim).
Currently there is only a playtest build, but if you want to get notified, when the game gets released, you can [wishlist it here](https://store.steampowered.com/app/3687560/sfsim/).

Anyway, let me know any comments and suggestions.

Enjoy!

## Related blog posts

* [Flight dynamics model for simulating Venturestar style spacecraft](https://www.wedesoft.de/simulation/2025/06/06/flight-model-physics-venturestar/)
* [Test Driven Development with OpenGL](https://www.wedesoft.de/software/2022/07/01/tdd-with-opengl/)
* [Implementing GUIs using Clojure and LWJGL Nuklear bindings](https://www.wedesoft.de/software/2024/05/11/clojure-nuklear/)
* [Procedural Volumetric Clouds](https://www.wedesoft.de/software/2023/05/03/volumetric-clouds/)
* [Procedural generation of global cloud cover](https://www.wedesoft.de/software/2023/03/20/procedural-global-cloud-cover/)
* [Reversed-Z Rendering in OpenGL](https://www.wedesoft.de/software/2021/09/20/reversed-z-rendering/)
* [Specifying Clojure function schemas with Malli](https://www.wedesoft.de/software/2023/12/25/clojure-function-schemas-with-malli/)
* [Implement an Interpreter using Clojure Instaparse](https://www.wedesoft.de/software/2024/07/05/clojure-instaparse/)
* [Orbits with Jolt Physics](https://www.wedesoft.de/simulation/2025/08/09/orbits-with-jolt-physics/)
* [Getting started with the Jolt Physics Engine](https://www.wedesoft.de/simulation/2024/09/26/jolt-physics-engine/)
* [Create Blender bones and animate and import with Assimp](https://www.wedesoft.de/graphics/2023/09/29/blender-animate-bones-assimp/)

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
[20]: https://ebruneton.github.io/precomputed_atmospheric_scattering/
[21]: https://docs.oracle.com/javase/8/docs/api/java/nio/ByteBuffer.html
[22]: https://clojuredocs.org/clojure.java.io/output-stream
[23]: https://clojuredocs.org/clojure.core/pmap
[24]: https://github.com/clojure-goes-fast/clj-async-profiler
[25]: https://visibleearth.nasa.gov/collection/1484/blue-marble
[26]: https://earthobservatory.nasa.gov/features/NightLights/page3.php
[27]: https://www.ngdc.noaa.gov/mgg/topo/gltiles.html
[28]: https://rhodesmill.org/skyfield/
[29]: https://naif.jpl.nasa.gov/naif/index.html
[30]: https://en.wikipedia.org/wiki/Chebyshev_polynomials
[31]: https://clojure.org/guides/tools_build
[32]: https://github.com/clj-commons/gloss/wiki/Introduction
