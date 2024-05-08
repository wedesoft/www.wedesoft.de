---
layout: post
title: Implementing GUIs using Clojure and LWJGL Nuklear bindings
category: software
image: /pics/nuklear.png
---

<span class="center"><img src="/pics/nuklear.png" width="508" alt="Nuklear GUI"/></span>

For game development I have been using [LWJGL3][1] which is a great Java library for cross-platform development.
Among other things it has bindings for *OpenGL*, *GLFW*, and *STB*.
Recently I discovered that it also has *Nuklear* bindings.
[Nuklear][2] is a small C library useful for developing GUIs for games.
It receives control input and commands to populate a GUI and converts those into render instructions.
Nuklear focuses solely on the user interface, while input and graphics backend are handled by the application.
It is therefore very flexible and can be integrated into a 3D game implemented using OpenGL, DirectX, Vulkan, or other.

LWJGL Nuklear bindings come with the [GLFWDemo.java][3] example.
In this article I have basically translated the input and graphics backend to Clojure.
I also added examples for several different controls.
I have [pushed the source code to Github][4] if you want to look at it straight away.

The demo is more than 400 lines of code.
This is because it has to implement the graphics backend, input conversion, and truetype font conversion to bitmap font.
If you are rather looking for a Clojure GUI library which does not require you to do this, you might want to look at [HumbleUI][7].

### Dependencies

Here is the *deps.edn* file:
{% highlight clojure %}
{:deps {org.clojure/clojure {:mvn/version "1.11.3"}
        org.lwjgl/lwjgl {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl$natives-linux {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-opengl {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-opengl$natives-linux {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-nuklear {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-nuklear$natives-linux {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-glfw {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-glfw$natives-linux {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-stb {:mvn/version "3.3.3"}
        org.lwjgl/lwjgl-stb$natives-linux {:mvn/version "3.3.3"}}
 :paths ["."]}
{% endhighlight %}
The file contains dependencies to:
* LWJGL core library
* OpenGL for rendering
* Nuklear for the GUI
* GLFW for creating a window and handling input
* STB for loading images and for converting a truetype font to a texture

If you are not using the *natives-linux* bindings, there are more [native packages for lwjgl-opengl][6] such as *natives-windows* and *natives-macos*.

### GLFW Window

We start with a simple program showing a window which can be closed by the user.
Here is the initial *nukleartest.clj* file:
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl.glfw GLFW]
             [org.lwjgl.opengl GL]))

(def width 640)
(def height 640)

(GLFW/glfwInit)

(GLFW/glfwDefaultWindowHints)
(GLFW/glfwWindowHint GLFW/GLFW_RESIZABLE GLFW/GLFW_FALSE)
(def window (GLFW/glfwCreateWindow width height "Nuklear Example" 0 0))

(GLFW/glfwMakeContextCurrent window)
(GLFW/glfwShowWindow window)
(GL/createCapabilities)
(GLFW/glfwSwapInterval 1)

(while (not (GLFW/glfwWindowShouldClose window))
       (GLFW/glfwPollEvents)
       (GLFW/glfwSwapBuffers window))

(GLFW/glfwTerminate)

(System/exit 0)
{% endhighlight %}
You can run the program using *clj -M -m nukleartest* and it should show a blank window.

<span class="center"><img src="/pics/glfw.png" width="508" alt="GLFW Window"/></span>

### OpenGL Shader Program

Next thing is to add initialisation of an OpenGL shader program to be used in the rendering backend.
The code is similar to my [earlier post showing an OpenGL Clojure example][5].

{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl.glfw GLFW]
             [org.lwjgl.opengl GL GL20]))

; ...

(def vertex-source
"#version 410 core
uniform mat4 projection;
in vec2 position;
in vec2 texcoord;
in vec4 color;
out vec2 frag_uv;
out vec4 frag_color;
void main()
{
  frag_uv = texcoord;
  frag_color = color;
  gl_Position = projection * vec4(position, 0, 1);
}")

(def fragment-source
"#version 410 core
uniform sampler2D tex;
in vec2 frag_uv;
in vec4 frag_color;
out vec4 out_color;
void main()
{
  out_color = frag_color * texture(tex, frag_uv);
}")

(def program (GL20/glCreateProgram))

(def vertex-shader (GL20/glCreateShader GL20/GL_VERTEX_SHADER))
(GL20/glShaderSource vertex-shader vertex-source)
(GL20/glCompileShader vertex-shader)
(when (zero? (GL20/glGetShaderi vertex-shader GL20/GL_COMPILE_STATUS))
  (println (GL20/glGetShaderInfoLog vertex-shader 1024))
  (System/exit 1))

(def fragment-shader (GL20/glCreateShader GL20/GL_FRAGMENT_SHADER))
(GL20/glShaderSource fragment-shader fragment-source)
(GL20/glCompileShader fragment-shader)
(when (zero? (GL20/glGetShaderi fragment-shader GL20/GL_COMPILE_STATUS))
  (println (GL20/glGetShaderInfoLog fragment-shader 1024))
  (System/exit 1))

(GL20/glAttachShader program vertex-shader)
(GL20/glAttachShader program fragment-shader)
(GL20/glLinkProgram program)
(when (zero? (GL20/glGetProgrami program GL20/GL_LINK_STATUS))
  (println (GL20/glGetProgramInfoLog program 1024))
  (System/exit 1))
(GL20/glDeleteShader vertex-shader)
(GL20/glDeleteShader fragment-shader)

; ...

(GL20/glDeleteProgram program)

; ...
{% endhighlight %}

<span class="center"><img src="/pics/shaders.svg" width="508" alt="vertex and fragment shader"/></span>

The vertex shader passes through texture coordinates and fragment colors.
Furthermore it scales the input position to OpenGL normalized device coordinates (we will set the projection matrix later).
The fragment shader performs a texture lookup and multiplies the result with the fragment color value.
The Clojure code compiles and links the shaders and checks for possible errors.

### Vertex Array Object

Next an OpenGL vertex array object is defined.
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl.glfw GLFW]
             [org.lwjgl.opengl GL GL11 GL15 GL20 GL30]))

; ...

(GL20/glUseProgram program)

(def position (GL20/glGetAttribLocation program "position"))
(def texcoord (GL20/glGetAttribLocation program "texcoord"))
(def color (GL20/glGetAttribLocation program "color"))

(def vbo (GL15/glGenBuffers))
(def ebo (GL15/glGenBuffers))
(def vao (GL30/glGenVertexArrays))

(GL30/glBindVertexArray vao)
(GL15/glBindBuffer GL15/GL_ARRAY_BUFFER vbo)
(GL15/glBindBuffer GL15/GL_ELEMENT_ARRAY_BUFFER ebo)

(GL20/glEnableVertexAttribArray position)
(GL20/glEnableVertexAttribArray texcoord)
(GL20/glEnableVertexAttribArray color)

(GL20/glVertexAttribPointer position 2 GL11/GL_FLOAT false 20 0)
(GL20/glVertexAttribPointer texcoord 2 GL11/GL_FLOAT false 20 8)
(GL20/glVertexAttribPointer color 4 GL11/GL_UNSIGNED_BYTE true 20 16)

; ...

(GL30/glBindVertexArray 0)
(GL15/glBindBuffer GL15/GL_ARRAY_BUFFER 0)
(GL15/glBindBuffer GL15/GL_ELEMENT_ARRAY_BUFFER 0)

(GL30/glDeleteVertexArrays vao)
(GL15/glDeleteBuffers ebo)
(GL15/glDeleteBuffers vbo)

; ...
{% endhighlight %}
An array buffer containing the position, texture coordinates, and colors is allocated.
Furthermore an element array buffer is allocated which contains element indices.
A row in the array buffer contains 20 bytes:
* 2 times 4 bytes for floating point "position"
* 2 times 4 bytes for floating point texture coordinate "texcoord"
* 4 bytes for RGBA color value "color"

<span class="center"><img src="/pics/vao.svg" width="508" alt="vertex buffer object"/></span>

The Nuklear library needs to be configured with the same layout of the vertex array buffer.
For this purpose a Nuklear vertex layout object is initialised using the *NK\_VERTEX\_ATTRIBUTE\_COUNT* attribute as a terminator:
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.nuklear Nuklear NkDrawVertexLayoutElement]))

; ...

(def vertex-layout (NkDrawVertexLayoutElement/malloc 4))
(-> vertex-layout (.position 0) (.attribute Nuklear/NK_VERTEX_POSITION)
    (.format Nuklear/NK_FORMAT_FLOAT) (.offset 0))
(-> vertex-layout (.position 1) (.attribute Nuklear/NK_VERTEX_TEXCOORD)
    (.format Nuklear/NK_FORMAT_FLOAT) (.offset 8))
(-> vertex-layout (.position 2) (.attribute Nuklear/NK_VERTEX_COLOR)
    (.format Nuklear/NK_FORMAT_R8G8B8A8) (.offset 16))
(-> vertex-layout (.position 3)
    (.attribute Nuklear/NK_VERTEX_ATTRIBUTE_COUNT)
    (.format Nuklear/NK_FORMAT_COUNT) (.offset 0))
(.flip vertex-layout)

; ...
{% endhighlight %}

### Null Texture

For drawing flat colors using the shader program above, Nuklear needs to specify a null texture.
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl BufferUtils]
             ; ...
             [org.lwjgl.nuklear Nuklear NkDrawVertexLayoutElement
              NkDrawNullTexture]))

; ...

(def null-tex (GL11/glGenTextures))
(GL11/glBindTexture GL11/GL_TEXTURE_2D null-tex)
(def buffer (BufferUtils/createIntBuffer 1))
(.put buffer (int-array [0xFFFFFFFF]))
(.flip buffer)
(GL11/glTexImage2D GL11/GL_TEXTURE_2D 0 GL11/GL_RGBA8 1 1 0 GL11/GL_RGBA
                   GL11/GL_UNSIGNED_BYTE buffer)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MIN_FILTER
                      GL11/GL_NEAREST)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MAG_FILTER
                      GL11/GL_NEAREST)

(def null-texture (NkDrawNullTexture/create))
(.id (.texture null-texture) null-tex)
(.set (.uv null-texture) 0.5 0.5)

; ...

(GL11/glDeleteTextures null-tex)

; ...
{% endhighlight %}
The null texture basically just consists of a single white pixel so that the shader term `texture(tex, frag_uv)` evaluates to `vec4(1, 1, 1, 1)`.
The Nuklear null texture uses fixed texture coordinates for lookup (here: 0.5, 0.5).
I.e. it is possible to embed the null texture in a bigger multi-purpose texture to save a texture slot.

### Nuklear Context, Command Buffer, and Configuration

Finally we can set up a Nuklear context object "context", a render command buffer "cmds", and a rendering configuration "config".
Nuklear even delegates allocating and freeing up memory, so we need to register callbacks for that as well.
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.nuklear ; ...
              NkAllocator NkContext NkPluginAllocI NkPluginFreeI NkUserFont
              NkBuffer NkConvertConfig]
             [org.lwjgl.system MemoryUtil MemoryStack]))

; ...

(def buffer-initial-size (* 4 1024))

(def stack (MemoryStack/stackPush))

; ...

(def allocator (NkAllocator/create))
(.alloc allocator
  (reify NkPluginAllocI
         (invoke [this handle old size] (MemoryUtil/nmemAllocChecked size))))
(.mfree allocator
  (reify NkPluginFreeI
         (invoke [this handle ptr] (MemoryUtil/nmemFree ptr))))

(def context (NkContext/create))

(def font (NkUserFont/create))
(Nuklear/nk_init context allocator font)

(def cmds (NkBuffer/create))
(Nuklear/nk_buffer_init cmds allocator buffer-initial-size)

(def config (NkConvertConfig/calloc stack))

(doto config
      (.vertex_layout vertex-layout)
      (.vertex_size 20)
      (.vertex_alignment 4)
      (.tex_null null-texture)
      (.circle_segment_count 22)
      (.curve_segment_count 22)
      (.arc_segment_count 22)
      (.global_alpha 1.0)
      (.shape_AA Nuklear/NK_ANTI_ALIASING_ON)
      (.line_AA Nuklear/NK_ANTI_ALIASING_ON))

; ...

(Nuklear/nk_free context)

(.free (.alloc allocator))
(.free (.mfree allocator))

; ...
{% endhighlight %}
We also created an empty font object which we will initialise properly later.

### Setup Rendering
OpenGL needs to be configured for rendering the Nuklear GUI.
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.opengl GL GL11 GL13 GL14 GL15 GL20 GL30]
             ; ...
             ))

; ...

(GL11/glEnable GL11/GL_BLEND)
(GL14/glBlendEquation GL14/GL_FUNC_ADD)
(GL14/glBlendFunc GL14/GL_SRC_ALPHA GL14/GL_ONE_MINUS_SRC_ALPHA)
(GL11/glDisable GL11/GL_CULL_FACE)
(GL11/glDisable GL11/GL_DEPTH_TEST)
(GL11/glEnable GL11/GL_SCISSOR_TEST)
(GL13/glActiveTexture GL13/GL_TEXTURE0)

(def projection (GL20/glGetUniformLocation program "projection"))
(def buffer (BufferUtils/createFloatBuffer 16))
(.put buffer (float-array [(/ 2.0 width) 0.0 0.0 0.0,
                           0.0 (/ -2.0 height) 0.0 0.0,
                           0.0 0.0 -1.0 0.0,
                           -1.0 1.0 0.0 1.0]))
(.flip buffer)
(GL20/glUniformMatrix4fv projection false buffer)

; ...
{% endhighlight %}
* Blending with existing pixel data is enabled and the blending equation and function are set
* Culling of back or front faces is disabled
* Depth testing is disabled
* Scissor testing is enabled
* The first texture slot is enabled

Also the uniform projection matrix for mapping pixel coordinates \[0, width\] x \[0, height\] to \[-1, 1\] x \[-1, 1\] is defined.
The projection matrix also flips the y-coordinates since the direction of the OpenGL y-axis is reversed in relation to the pixel y-coordinates.

<span class="center"><img src="/pics/window-to-ndc.svg" width="508" alt="Converting window coordinates to normalized device coordinates"/></span>

### Nuklear GUI

Now we will add a minimal GUI just using a progress bar for testing rendering without fonts.
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl BufferUtils PointerBuffer]
             ; ...
             [org.lwjgl.nuklear ; ...
              NkRect]
             ; ...
             ))

; ...

(def rect (NkRect/malloc stack))

(def progress (PointerBuffer/allocateDirect 1))

(while (not (GLFW/glfwWindowShouldClose window))
       (Nuklear/nk_input_begin context)
       (GLFW/glfwPollEvents)
       (Nuklear/nk_input_end context)
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         (.put progress 0 (mod (inc (.get progress 0)) 100))
         (Nuklear/nk_layout_row_dynamic context 32 1)
         (Nuklear/nk_progress context progress 100 true)
         (Nuklear/nk_end context))
       ; ...
       )

; ...
{% endhighlight %}
First we set up a few values and then in the main loop we start Nuklear input using `Nuklear/nk_input_begin`, call GLFW to process events, and then end Nuklear input.
We will implement the GLFW callbacks to convert events to Nuklear calls later.

We start populating the GUI by calling `Nuklear/nk_begin` thereby specifying the window size.
We increase the progress value and store it in a PointerBuffer object.
The call `(Nuklear/nk_layout_row_dynamic context 32 1)` sets the GUI layout to 32 pixels height and one widget per row.
Then a progress bar is created and the GUI is finalised using `Nuklear/nk_end`.

### Rendering Backend

Now we are ready to add the rendering backend.
{% highlight clojure %}
; ...

(def max-vertex-buffer (* 512 1024))
(def max-element-buffer (* 128 1024))

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (GL11/glViewport 0 0 width height)
       (GL15/glBufferData GL15/GL_ARRAY_BUFFER max-vertex-buffer
                          GL15/GL_STREAM_DRAW)
       (GL15/glBufferData GL15/GL_ELEMENT_ARRAY_BUFFER max-element-buffer
                          GL15/GL_STREAM_DRAW)
       (let [vertices (GL15/glMapBuffer GL15/GL_ARRAY_BUFFER
                                        GL15/GL_WRITE_ONLY max-vertex-buffer
                                        nil)
             elements (GL15/glMapBuffer GL15/GL_ELEMENT_ARRAY_BUFFER
                                        GL15/GL_WRITE_ONLY max-element-buffer
                                        nil)
             stack    (MemoryStack/stackPush)
             vbuf     (NkBuffer/malloc stack)
             ebuf     (NkBuffer/malloc stack)]
         (Nuklear/nk_buffer_init_fixed vbuf vertices)
         (Nuklear/nk_buffer_init_fixed ebuf elements)
         (Nuklear/nk_convert context cmds vbuf ebuf config)
         (GL15/glUnmapBuffer GL15/GL_ELEMENT_ARRAY_BUFFER)
         (GL15/glUnmapBuffer GL15/GL_ARRAY_BUFFER)
         (loop [cmd (Nuklear/nk__draw_begin context cmds) offset 0]
               (when cmd
                 (when (not (zero? (.elem_count cmd)))
                   (GL11/glBindTexture GL11/GL_TEXTURE_2D
                                       (.id (.texture cmd)))
                   (let [clip-rect (.clip_rect cmd)]
                     (GL11/glScissor (int (.x clip-rect))
                                     (int (- height
                                             (int (+ (.y clip-rect)
                                                     (.h clip-rect)))))
                                     (int (.w clip-rect))
                                     (int (.h clip-rect))))
                   (GL11/glDrawElements GL11/GL_TRIANGLES (.elem_count cmd)
                                        GL11/GL_UNSIGNED_SHORT offset))
                 (recur (Nuklear/nk__draw_next cmd cmds context)
                        (+ offset (* 2 (.elem_count cmd))))))
         (Nuklear/nk_clear context)
         (Nuklear/nk_buffer_clear cmds)
         (GLFW/glfwSwapBuffers window)
         (MemoryStack/stackPop)))

; ...
{% endhighlight %}
The rendering backend sets the viewport and then array buffers for the vertex data and the indices is allocated.
Then the buffers are mapped to memory resulting in the two java.nio.DirectByteBuffer objects "vertices" and "elements".
The two static buffers are then converted to Nuklear buffer objects using `Nuklear/nk_buffer_init_fixed`.

Then the core method of the Nuklear library `Nuklear/nk_convert` is called. It populates the (dynamic) command buffer "cmds" which we initialised earlier as well as the mapped vertex buffer and index buffer.
After the conversion, the two OpenGL memory mappings are undone.

A Clojure loop then is used to get chunks of type NkDrawCommand from the render buffer.
Each draw command requires setting the texture id and the clipping region.
Then a part of the index and vertex buffer is rendered using `GL11/glDrawElements`.

Finally `Nuklear/nk_clear` is used to reset the GUI specification for the next frame and `Nuklear/nk_buffer_clear` is used to empty the command buffer.
`GLFW/glfwSwapBuffers` is used to publish the new rendered frame.

<span class="center"><img src="/pics/progress.png" width="508" alt="GUI showing a progress bar"/></span>

### Mouse events

The next step one can do is converting GLFW mouse events to Nuklear input.
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.glfw GLFW GLFWCursorPosCallbackI
              GLFWMouseButtonCallbackI]
             ; ...
             ))

; ...

(GLFW/glfwSetCursorPosCallback
  window
  (reify GLFWCursorPosCallbackI
         (invoke [this window xpos ypos]
           (Nuklear/nk_input_motion context (int xpos) (int ypos)))))

(GLFW/glfwSetMouseButtonCallback
  window
  (reify GLFWMouseButtonCallbackI
         (invoke [this window button action mods]
           (let [stack (MemoryStack/stackPush)
                 cx    (.mallocDouble stack 1)
                 cy    (.mallocDouble stack 1)]
             (GLFW/glfwGetCursorPos window cx cy)
             (let [x        (int (.get cx 0))
                   y        (int (.get cy 0))
                   nkbutton (cond
                              (= button GLFW/GLFW_MOUSE_BUTTON_RIGHT)
                                Nuklear/NK_BUTTON_RIGHT
                              (= button GLFW/GLFW_MOUSE_BUTTON_MIDDLE)
                                Nuklear/NK_BUTTON_MIDDLE
                              :else
                                Nuklear/NK_BUTTON_LEFT)]
               (Nuklear/nk_input_button context nkbutton x y
                                        (= action GLFW/GLFW_PRESS))
               (MemoryStack/stackPop))))))

; ...
{% endhighlight %}
The first callback is to process mouse cursor movement events.
The second callback converts mouse button press and release events to Nuklear input.
The progress bar is modifyable and you should now be able to change it by clicking on it.
Note that using a case statement instead of cond did not work for some reason.

### Converting Truetype Font to Bitmap Font

To display other GUI controls, text output is required.
Using the [STB][8] library a Truetype font is converted to a bitmap font of the desired size.
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.stb STBTruetype STBTTFontinfo STBTTPackedchar
              STBTTPackContext STBImageWrite]))

; ...

(def font-height 18)
(def bitmap-w 512)
(def bitmap-h 512)

; ...

(def font (NkUserFont/create))

(def ttf-in (clojure.java.io/input-stream "FiraSans.ttf"))
(def ttf-out (java.io.ByteArrayOutputStream.))
(clojure.java.io/copy ttf-in ttf-out)
(def ttf-bytes (.toByteArray ttf-out))
(def ttf (BufferUtils/createByteBuffer (count ttf-bytes)))
(.put ttf ttf-bytes)
(.flip ttf)

(def fontinfo (STBTTFontinfo/create))
(def cdata (STBTTPackedchar/calloc 95))

(STBTruetype/stbtt_InitFont fontinfo ttf)
(def scale (STBTruetype/stbtt_ScaleForPixelHeight fontinfo font-height))

(def d (.mallocInt stack 1))
(STBTruetype/stbtt_GetFontVMetrics fontinfo nil d nil)
(def descent (* (.get d 0) scale))
(def bitmap (MemoryUtil/memAlloc (* bitmap-w bitmap-h)))
(def pc (STBTTPackContext/malloc stack))
(STBTruetype/stbtt_PackBegin pc bitmap bitmap-w bitmap-h 0 1 0)
(STBTruetype/stbtt_PackSetOversampling pc 4 4)
(STBTruetype/stbtt_PackFontRange pc ttf 0 font-height 32 cdata)
(STBTruetype/stbtt_PackEnd pc)

(def texture (MemoryUtil/memAlloc (* bitmap-w bitmap-h 4)))
(def data (byte-array (* bitmap-w bitmap-h)))
(.get bitmap data)
(def data (int-array (mapv #(bit-or (bit-shift-left % 24) 0x00FFFFFF) data)))
(def texture-int (.asIntBuffer texture))
(.put texture-int data)

; (STBImageWrite/stbi_write_png "font.png" bitmap-w bitmap-h 4 texture
;                               (* 4 bitmap-w))

; ...
{% endhighlight %}
Basically the font file is read and converted to a `java.nio.DirectByteBuffer` (let me know if you find a more straightforward way to do this).
The data is used to initialise a STB font info object.
The next steps I can't explain in detail but they basically pack the glyphs into a greyscale bitmap.

Finally a white RGBA texture data is created with the greyscale bitmap as the alpha channel.
You can write out the RGBA data to a PNG file and inspect it using GIMP or your favourite image editor.

<span class="center"><img src="/pics/font.png" width="508" alt="Bitmap font created with STB library"/></span>

[1]: https://www.lwjgl.org/
[2]: https://immediate-mode-ui.github.io/Nuklear/doc/index.html
[3]: https://github.com/LWJGL/lwjgl3/blob/master/modules/samples/src/test/java/org/lwjgl/demo/nuklear/GLFWDemo.java
[4]: https://github.com/wedesoft/nukleartest
[5]: https://www.wedesoft.de/software/2023/05/26/lwjgl3-clojure/
[6]: https://repo1.maven.org/maven2/org/lwjgl/lwjgl-opengl/3.3.2/
[7]: https://github.com/HumbleUI/HumbleUI
[8]: https://github.com/nothings/stb
