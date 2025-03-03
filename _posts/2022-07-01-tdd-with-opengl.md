---
layout: post
title: Test Driven Development with OpenGL
category: software
image: /pics/glxgears.png
---

<span class="center"><img src="/pics/glxgears.png" width="128"></span>

## Test Driven Development

Test driven development (TDD) undoubtedly helps a great deal in preventing development grinding to a halt once a project's size surpasses a few lines of code.

1. The reason for first writing a failing test is to ensure that the test is actually failing and testing the next code change.
1. A minimal change to the code is performed to pass the new test while also still passing all previously written tests.
1. If necessary the code is refactored/simplified. The reason to do this after passing the test is so that one does not have to worry about passing the test and writing clean code at the same time.

## Testing rendering output

One can test OpenGL programs by rendering test images and comparing them with a saved image (a test fixture).
In order to automate this, one can perform offscreen rendering and do a pixel-wise image comparison with the saved image.

Using the [Clojure][2] programming language and the [Lightweight Java Game Library (LWJGL)][1] one can perform offscreen rendering with a Pbuffer object using the following macro (of course this approach is not limited to Clojure and LWJGL):

{% highlight clojure %}
; Set up OpenGL modes
(defn setup-rendering
  "Common code for setting up rendering"
  [width height]
  (GL11/glViewport 0 0 width height)
  (GL11/glEnable GL11/GL_DEPTH_TEST)
  (GL11/glEnable GL11/GL_CULL_FACE)
  (GL11/glCullFace GL11/GL_BACK)
  (GL11/glDepthFunc GL11/GL_GEQUAL)
  (GL45/glClipControl GL20/GL_LOWER_LEFT GL45/GL_ZERO_TO_ONE))

; Macro accepting a block of code to be rendered to a Pbuffer
(defmacro offscreen-render
  "Macro to use a pbuffer for offscreen rendering"
  [width height & body]
  `(let [pixels#  (BufferUtils/createIntBuffer (* ~width ~height))
         pbuffer# (Pbuffer. ~width ~height (PixelFormat. 24 8 24 0 0) nil nil)
         data#    (int-array (* ~width ~height))]
     (.makeCurrent pbuffer#)
     (setup-rendering ~width ~height)
     (try
       ~@body
       (GL11/glReadPixels 0 0 ~width ~height GL12/GL_BGRA GL11/GL_UNSIGNED_BYTE pixels#)
       (.get pixels# data#)
       {:width ~width :height ~height :data data#}
       (finally
         (.releaseContext pbuffer#)
         (.destroy pbuffer#)))))
{% endhighlight %}

Note that the code sets up [reversed-z rendering as discussed in an earlier article][5]

Using the [Midje][4] testing library one can for example test a command for clearing the color buffer as follows:

{% highlight clojure %}
; Test rendering a red background
(fact "Render background color"
  (offscreen-render 160 120 (clear (matrix [1.0 0.0 0.0])))
  => (is-image "test/sfsim25/fixtures/render/red.png"))
{% endhighlight %}

The checker `is-image` is implemented using [ImageJ][3]:

{% highlight clojure %}
; Implement equality test for RGBA images ignoring alpha channel.
(defn is-image
  "Compare RGB components of image and ignore alpha values."
  [filename]
  (fn [other]
      (let [img (slurp-image filename)]
        (and (= (:width img) (:width other))
             (= (:height img) (:height other))
             (= (map #(bit-and % 0x00ffffff) (:data img))
                (map #(bit-and % 0x00ffffff) (:data other)))))))

; Load an RGB image
(defn slurp-image
  "Load an RGB image"
  [^String file-name]
  (let [img (.openImage (Opener.) file-name)]
    (.convertToRGB (ImageConverter. img))
    {:width (.getWidth img)
     :height (.getHeight img)
     :data (.getPixels (.getProcessor img))}))
{% endhighlight %}

The image is recorded initially by using the checker `record-image` instead of `is-image` and verifying the result manually.

{% highlight clojure %}
; Test method writing image to file instead of comparing it.
(defn record-image
  "Use this test function to record the image the first time."
  [filename]
  (fn [other]
      (spit-image filename other)))

; Method to save image.
(defn spit-image
  "Save RGB image as PNG file"
  [^String file-name {:keys [width height data]}]
  (let [processor (ColorProcessor. width height data)
        img       (ImagePlus.)]
    (.setProcessor img processor)
    (.saveAsPng (FileSaver. img) file-name)))
{% endhighlight %}

One can use this approach (and maybe only this approach) to test code for handling vertex array objects, textures, and for loading shaders.

## Testing shader code

Above approach has the drawback that it can only test complete rendering programs.
Also the output is limited to 24-bit RGB images.
The tests are therefore more like integration tests and they are not suitable for unit testing shader functions.

However it is possible to use a Pbuffer just as a rendering context and perform rendering to a floating-point texture.
One can use a texture with a single pixel as a framebuffer.
A single pixel of a uniformly colored quad is drawn.
The floating point channels of the texture's RGB pixel then can be compared with the expected value.

{% highlight clojure %}
; Use shaders to render to a single pixel texture and read out result.
(defn shader-test [setup probe & shaders]
  (fn [uniforms args]
      (let [result (promise)]
        (offscreen-render 1 1
          (let [indices  [0 1 3 2]
                vertices [-1.0 -1.0 0.5, 1.0 -1.0 0.5, -1.0 1.0 0.5, 1.0 1.0 0.5]
                program  (make-program :vertex [vertex-passthrough]
                                       :fragment (conj shaders (apply probe args)))
                vao      (make-vertex-array-object program indices vertices [:point 3])
                tex      (texture-render 1 1 true
                                         (use-program program)
                                         (apply setup program uniforms)
                                         (render-quads vao))
                img      (texture->vectors tex 1 1)]
            (deliver result (get-vector img 0 0))
            (destroy-texture tex)
            (destroy-vertex-array-object vao)
            (destroy-program program)))
        @result)))

; Minimal vertex shader.
(def vertex-passthrough "#version 410 core
in highp vec3 point;
void main()
{
  gl_Position = vec4(point, 1);
}")

; Macro to render output of a code block to a texture.
(defmacro texture-render
  "Macro to render to a texture"
  [width height floating-point & body]
  `(let [fbo# (GL45/glCreateFramebuffers)
         tex# (GL11/glGenTextures)]
     (try
       (GL30/glBindFramebuffer GL30/GL_FRAMEBUFFER fbo#)
       (GL11/glBindTexture GL11/GL_TEXTURE_2D tex#)
       (GL42/glTexStorage2D GL11/GL_TEXTURE_2D 1
         (if ~floating-point GL30/GL_RGB32F GL11/GL_RGBA8) ~width ~height)
       (GL32/glFramebufferTexture GL30/GL_FRAMEBUFFER GL30/GL_COLOR_ATTACHMENT0 tex# 0)
       (GL20/glDrawBuffers (make-int-buffer (int-array [GL30/GL_COLOR_ATTACHMENT0])))
       (GL11/glViewport 0 0 ~width ~height)
       ~@body
       {:texture tex# :target GL11/GL_TEXTURE_2D}
       (finally
         (GL30/glBindFramebuffer GL30/GL_FRAMEBUFFER 0)
         (GL30/glDeleteFramebuffers fbo#)))))

; Read out texture and return floating point RGB image.
(defn texture->vectors
  "Extract floating-point vectors from texture"
  [texture width height]
  (with-2d-texture (:texture texture)
    (let [buf  (BufferUtils/createFloatBuffer (* width height 3))
          data (float-array (* width height 3))]
      (GL11/glGetTexImage GL11/GL_TEXTURE_2D 0 GL12/GL_BGR GL11/GL_FLOAT buf)
      (.get buf data)
      {:width width :height height :data data})))
{% endhighlight %}

Furthermore it is possible to compose the fragment shader by linking the shader function under test with a main function.
I.e. it is possible to link the shader function under test with a main function implemented just for probing the shader.

The `shader-test` function defines a test function using the probing shader and the shader under test.
The new test function then can be used using the Midje tabular environment.
In the following example the GLSL function `phase` is tested.
Note that parameters in the probing shaders are set using the [weavejester/comb][6] templating library.

{% highlight clojure %}
; Probing shader generated using templating.
(def phase-probe
  (template/fn [g mu] "#version 410 core
out lowp vec3 fragColor;
float phase(float g, float mu);
void main()
{
  float result = phase(<%= g %>, <%= mu %>);
  fragColor = vec3(result, 0, 0);
}"))

; Use shader test function and probing shader to test a shadder.
(def phase-test (shader-test (fn [program]) phase-probe phase-function))

; Midje tests using test function to test a shader.
(tabular "Shader function for scattering phase function"
         (fact (mget (phase-test [] [?g ?mu]) 0) => (roughly ?result))
         ?g  ?mu ?result
         0   0   (/ 3 (* 16 PI))
         0   1   (/ 6 (* 16 PI))
         0  -1   (/ 6 (* 16 PI))
         0.5 0   (/ (* 3 0.75) (* 8 PI 2.25 (pow 1.25 1.5)))
         0.5 1   (/ (* 6 0.75) (* 8 PI 2.25 (pow 0.25 1.5))))
{% endhighlight %}

Note that using `mget` the red channel of the pixel is extracted.
Sometimes it might be more desirable to check all channels of the RGB pixel.

Here is the actual implementation of the tested function:

{% highlight glsl %}
#version 410 core

float M_PI = 3.14159265358;

float phase(float g, float mu)
{
  return 3 * (1 - g * g) * (1 + mu * mu) /
    (8 * M_PI * (2 + g * g) * pow(1 + g * g - 2 * g * mu, 1.5));
}
{% endhighlight %}

The empty function `(fn [program])` is specified as a setup function.
In general the setup function is used to initialise uniforms used in the shader under test.

Here is an example of tests using uniform values:

{% highlight clojure %}
; Probing shader generated using templating.
(def transmittance-track-probe
  (template/fn [px py pz qx qy qz] "#version 410 core
out lowp vec3 fragColor;
vec3 transmittance_track(vec3 p, vec3 q);
void main()
{
  vec3 p = vec3(<%= px %>, <%= py %>, <%= pz %>);
  vec3 q = vec3(<%= qx %>, <%= qy %>, <%= qz %>);
  fragColor = transmittance_track(p, q);
}"))

; Shader test involving some uniform variables.
(def transmittance-track-test
  (transmittance-shader-test
    (fn [program height-size elevation-size elevation-power radius max-height]
        (uniform-int program :height_size height-size)
        (uniform-int program :elevation_size elevation-size)
        (uniform-float program :elevation_power elevation-power)
        (uniform-float program :radius radius)
        (uniform-float program :max_height max-height))
    transmittance-track-probe transmittance-track
    shaders/transmittance-forward shaders/horizon-angle
    shaders/elevation-to-index shaders/interpolate-2d
    shaders/convert-2d-index shaders/is-above-horizon))

; Midje tests to test a shader.
(tabular "Shader function to compute transmittance between two points in the atmosphere"
         (fact (mget (transmittance-track-test [17 17 1 6378000.0 100000.0]
                                               [?px ?py ?pz ?qx ?qy ?qz]) 0)
               => (roughly ?result 1e-6))
         ?px ?py ?pz     ?qx ?qy ?qz     ?result
         0   0   6478000 0   0   6478000 1
         0   0   6428000 0   0   6478000 0.5
         0   0   6453000 0   0   6478000 0.75
         0   0   6428000 0   0   6453000 (/ 0.5 0.75))
{% endhighlight %}

Here a setup function initialising 5 uniform values is specified.

## Mocking shader functions

If each shader function is implemented as a separate string (loaded from a separate file), one can easily link with mock functions when testing shaders.
Here is an example of a probing shader which also contains mocks to allow the shader to be unit tested in isolation:

{% highlight clojure %}
; Probing shader generated using templating and defining mock shaders.
(def cloud-track-base-probe
  (template/fn [px qx n decay scatter density ir ig ib]
"#version 410 core
out lowp vec3 fragColor;
vec3 transmittance_forward(vec3 point, vec3 direction)
{
  float distance = 10 - point.x;
  float transmittance = exp(-<%= decay %> * distance);
  return vec3(transmittance, transmittance, transmittance);
}
vec3 ray_scatter_forward(vec3 point, vec3 direction, vec3 light)
{
  float distance = 10 - point.x;
  float amount = <%= scatter %> * (1 - pow(2, -distance));
  return vec3(0, 0, amount);
}
float cloud_density(vec3 point)
{
  return <%= density %>;
}
float phase(float g, float mu)
{
  return 1.0 + 0.5 * mu;
}
vec3 cloud_track_base(vec3 p, vec3 q, int n, vec3 incoming);
void main()
{
  vec3 p = vec3(<%= px %>, 0, 0);
  vec3 q = vec3(<%= qx %>, 0, 0);
  vec3 incoming = vec3(<%= ir %>, <%= ig %>, <%= ib %>);
  fragColor = cloud_track_base(p, q, <%= n %>, incoming);
}
"))
{% endhighlight %}

Let me know if you have any comments or suggestions.

Enjoy!

**Updates:**

* submitted to [Reddit][7]
* submitted to [Hackernews][8]
* Rhawk187 pointed out that exact image comparisons are also problematic because updates to graphics drivers can cause subtle changes. This can be adressed by allowing a small average difference between the expected and actual image.
* In order to be independent of the graphics driver one can run the tests using `xvfb-run` to use software rendering instead.

The code of `make-vertex-array-object` and `render-quads` is added here for reference.

{% highlight clojure %}
(defn make-vertex-array-object
  "Create vertex array object and vertex buffer objects"
  [program indices vertices attributes]
  (let [vertex-array-object (GL30/glGenVertexArrays)]
    (GL30/glBindVertexArray vertex-array-object)
    (let [array-buffer (GL15/glGenBuffers)
          index-buffer (GL15/glGenBuffers)]
      (GL15/glBindBuffer GL15/GL_ARRAY_BUFFER array-buffer)
      (GL15/glBufferData GL15/GL_ARRAY_BUFFER
                         (make-float-buffer (float-array vertices)) GL15/GL_STATIC_DRAW)
      (GL15/glBindBuffer GL15/GL_ELEMENT_ARRAY_BUFFER index-buffer)
      (GL15/glBufferData GL15/GL_ELEMENT_ARRAY_BUFFER
                         (make-int-buffer (int-array indices)) GL15/GL_STATIC_DRAW)
      (let [attribute-pairs (partition 2 attributes)
            sizes           (map second attribute-pairs)
            stride          (apply + sizes)
            offsets         (reductions + (cons 0 (butlast sizes)))]
        (doseq [[i [attribute size] offset] (map list (range) attribute-pairs offsets)]
          (GL20/glVertexAttribPointer (GL20/glGetAttribLocation
                                        ^int program (name attribute))
                                        ^int size
                                      GL11/GL_FLOAT false
                                        ^int (* stride Float/BYTES)
                                        ^int (* offset Float/BYTES))
          (GL20/glEnableVertexAttribArray i))
        {:vertex-array-object vertex-array-object
         :array-buffer        array-buffer
         :index-buffer        index-buffer
         :nrows               (count indices)
         :ncols               (count attribute-pairs)}))))

(defn render-quads
  "Render one or more quads"
  [vertex-array-object]
  (setup-vertex-array-object vertex-array-object)
  (GL11/glDrawElements GL11/GL_QUADS
    ^int (:nrows vertex-array-object) GL11/GL_UNSIGNED_INT 0))
{% endhighlight %}

[1]: https://legacy.lwjgl.org/javadoc.html
[2]: https://clojuredocs.org/
[3]: https://imagej.nih.gov/ij/developer/api/ij/module-summary.html
[4]: https://cljdoc.org/d/midje/midje/
[5]: https://www.wedesoft.de/software/2021/09/20/reversed-z-rendering/
[6]: https://github.com/weavejester/comb
[7]: https://www.reddit.com/r/opengl/comments/vpbcuy/test_driven_development_with_opengl/
[8]: https://news.ycombinator.com/item?id=31953169
