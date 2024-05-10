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
There also is [Quil][9] which seems to be more about graphics and animations.

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

### Font Texture and Nuklear Callbacks

The RGBA bitmap font can now be converted to an OpenGL texture with linear interpolation and the texture id of the NkUserFont object is set.
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl.nuklear ; ...
              NkHandle]
             ; ...
             ))

; ...

(def font-tex (GL11/glGenTextures))
(GL11/glBindTexture GL11/GL_TEXTURE_2D font-tex)
(GL11/glTexImage2D GL11/GL_TEXTURE_2D 0 GL11/GL_RGBA8 bitmap-w bitmap-h 0
                   GL11/GL_RGBA GL11/GL_UNSIGNED_BYTE texture)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MIN_FILTER
                      GL11/GL_LINEAR)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MAG_FILTER
                      GL11/GL_LINEAR)
(MemoryUtil/memFree texture)
(MemoryUtil/memFree bitmap)

(def handle (NkHandle/create))
(.id handle font-tex)
(.texture font handle)

; ...

(GL11/glDeleteTextures font-tex)

; ...
{% endhighlight %}

To get the font working with Nuklear, callbacks for text width, text height, and glyph region in the texture need to be added.
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl.nuklear ; ...
              NkTextWidthCallbackI NkQueryFontGlyphCallbackI NkUserFontGlyph]
             ; ...
             [org.lwjgl.stb ; ...
              STBTTAlignedQuad]))

; ...

(.width font
  (reify NkTextWidthCallbackI
    (invoke [this handle h text len]
      (let [stack     (MemoryStack/stackPush)
            unicode   (.mallocInt stack 1)
            advance   (.mallocInt stack 1)
            glyph-len (Nuklear/nnk_utf_decode text
                        (MemoryUtil/memAddress unicode) len)
            result
            (loop [text-len glyph-len glyph-len glyph-len text-width 0.0]
                  (if (or (> text-len len)
                          (zero? glyph-len)
                          (= (.get unicode 0) Nuklear/NK_UTF_INVALID))
                    text-width
                    (do
                      (STBTruetype/stbtt_GetCodepointHMetrics fontinfo
                        (.get unicode 0) advance nil)
                      (let [text-width (+ text-width
                                          (* (.get advance 0) scale))
                            glyph-len  (Nuklear/nnk_utf_decode
                                         (+ text text-len)
                                         (MemoryUtil/memAddress unicode)
                                         (- len text-len))]
                        (recur (+ text-len glyph-len)
                               glyph-len
                               text-width)))))]
        (MemoryStack/stackPop)
        result))))

(.height font font-height)

(.query font
        (reify NkQueryFontGlyphCallbackI
               (invoke [this handle font-height glyph codepoint
                        next-codepoint]
                 (let [stack   (MemoryStack/stackPush)
                       x       (.floats stack 0.0)
                       y       (.floats stack 0.0)
                       q       (STBTTAlignedQuad/malloc stack)
                       advance (.mallocInt stack 1)]
                   (STBTruetype/stbtt_GetPackedQuad cdata bitmap-w bitmap-h
                                                    (- codepoint 32) x y q
                                                    false)
                   (STBTruetype/stbtt_GetCodepointHMetrics fontinfo
                     codepoint advance nil)
                   (let [ufg (NkUserFontGlyph/create glyph)]
                     (.width ufg (- (.x1 q) (.x0 q)))
                     (.height ufg (- (.y1 q) (.y0 q)))
                     (.set (.offset ufg)
                           (.x0 q)
                           (+ (.y0 q) font-height descent))
                     (.xadvance ufg (* (.get advance 0) scale))
                     (.set (.uv ufg 0) (.s0 q) (.t0 q))
                     (.set (.uv ufg 1) (.s1 q) (.t1 q)))
                   (MemoryStack/stackPop)))))

; ...
{% endhighlight %}
I don't fully understand yet how the "width" and "query" implementations work.
Hopefully I find a way to do a unit-tested reimplementation to get a better understanding later.

On a positive note though, at this point it is possible to render text.
In the following code we add a button for stopping and a button for starting the progress bar.
{% highlight clojure %}
; ...

(def progress (PointerBuffer/allocateDirect 1))
(def increment (atom 1))

(while (not (GLFW/glfwWindowShouldClose window))
       (Nuklear/nk_input_begin context)
       (GLFW/glfwPollEvents)
       (Nuklear/nk_input_end context)
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         (.put progress 0 (mod (+ (.get progress 0) @increment) 100))
         (Nuklear/nk_layout_row_dynamic context 32 1)
         (Nuklear/nk_progress context progress 100 true)
         (Nuklear/nk_layout_row_dynamic context 32 2)
         (if (Nuklear/nk_button_label context "Start")
           (reset! increment 1))
         (if (Nuklear/nk_button_label context "Stop")
           (reset! increment 0))
         (Nuklear/nk_end context))
       ; ...
       )

; ...
{% endhighlight %}
The GUI now looks like this:

<span class="center"><img src="/pics/start-stop.png" width="508" alt="Progress bar with start and stop button"/></span>

### Trying out Widgets

#### Menubar

The following code adds a main menu with an exit item to the window.
{% highlight clojure %}
(ns nukleartest
    (:import [org.lwjgl.nuklear ; ...
              NkVec2]
             ; ...
             ))

; ...

(def menu-size (NkVec2/create))
(.x menu-size 80)
(.y menu-size 40)

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         (Nuklear/nk_menubar_begin context)
         (Nuklear/nk_layout_row_static context 40 40 1)
         (when (Nuklear/nk_menu_begin_label context "Main"
                                      Nuklear/NK_TEXT_LEFT menu-size)
           (Nuklear/nk_layout_row_dynamic context 32 1)
           (if (Nuklear/nk_menu_item_label context "Exit"
                                           Nuklear/NK_TEXT_LEFT)
             (GLFW/glfwSetWindowShouldClose window true))
           (Nuklear/nk_menu_end context))
         (Nuklear/nk_menubar_end context)
         ; ...
         ))

; ...
{% endhighlight %}
Here is a screenshot of the result:
<span class="center"><img src="/pics/menu.png" width="508" alt="Menubar"/></span>

#### Option Labels

One can add option labels to implement a choice between different options.
{% highlight clojure %}
; ...

(def option (atom :easy))

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         ; ...
         (Nuklear/nk_layout_row_dynamic context 32 3)
         (if (Nuklear/nk_option_label context "easy"
                                      (= @option :easy))
           (reset! option :easy))
         (if (Nuklear/nk_option_label context "intermediate"
                                      (= @option :intermediate))
           (reset! option :intermediate))
         (if (Nuklear/nk_option_label context "hard"
                                      (= @option :hard))
           (reset! option :hard))
         ; ...
         ))

; ...
{% endhighlight %}
Here is a screenshot with option labels showing the three options easy, intermediate, and hard.
<span class="center"><img src="/pics/option-labels.png" width="508" alt="Option Labels"/></span>

#### Check Labels

Check labels are easy to add. They look similar to the option labels, but use squares instead of circles.
{% highlight clojure %}
; ...

(def flip (atom false))
(def crop (atom false))

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         ; ...
         (Nuklear/nk_layout_row_dynamic context 32 2)
         (reset! flip (Nuklear/nk_check_label context "Flip" @flip))
         (reset! crop (Nuklear/nk_check_label context "Crop" @crop))
         ; ...
         ))

; ...
{% endhighlight %}
Here is a screenshot with two check labels.
<span class="center"><img src="/pics/check-labels.png" width="508" alt="Option Labels"/></span>

#### Property Widgets

Property widgets let you change a value by either clicking on the arrows or by clicking and dragging across the widget.
{% highlight clojure %}
; ...
(def compression (.put (BufferUtils/createIntBuffer 1) 0 20))
(def quality (.put (BufferUtils/createFloatBuffer 1) 0 (float 5.0)))
; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         ; ...
         (Nuklear/nk_property_int context "Compression:" 0 compression
                                  100 10 (float 1))
         (Nuklear/nk_property_float context "Quality:" (float 0.0) quality
                                    (float 10.0) (float 1.0) (float 0.01))
         ; ...
         ))

; ...
{% endhighlight %}
Here is a screenshot with an integer and a float property.
<span class="center"><img src="/pics/property.png" width="508" alt="Property Widgets"/></span>

#### Symbol and Image Buttons

Nuklear has several stock symbols for symbol buttons.
Furthermore one can register a texture to be used in an image button.
The button method returns true if it was clicked.
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.nuklear ; ...
              NkImage]
             ; ...
             [org.lwjgl.stb ; ...
              STBImage]))

; ...

(def download-icon (NkImage/create))
(def w (int-array 1))
(def h (int-array 1))
(def c (int-array 1))
(def buffer (STBImage/stbi_load "download.png" w h c 4))
(def download-tex (GL11/glGenTextures))
(GL11/glBindTexture GL11/GL_TEXTURE_2D download-tex)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MIN_FILTER
                      GL11/GL_LINEAR)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MAG_FILTER
                      GL11/GL_LINEAR)
(GL11/glTexImage2D GL11/GL_TEXTURE_2D 0 GL11/GL_RGBA8 (aget w 0) (aget h 0)
                   0 GL11/GL_RGBA GL11/GL_UNSIGNED_BYTE buffer)
(GL11/glBindTexture GL11/GL_TEXTURE_2D 0)
(def handle (NkHandle/create))
(.id handle download-tex)
(.handle download-icon handle)

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         ; ...
         (Nuklear/nk_layout_row_dynamic context 32 14)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_RECT_SOLID)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_RECT_OUTLINE)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_TRIANGLE_UP)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_TRIANGLE_DOWN)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_TRIANGLE_LEFT)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_TRIANGLE_RIGHT)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_CIRCLE_SOLID)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_CIRCLE_OUTLINE)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_MAX)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_X)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_PLUS)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_MINUS)
         (Nuklear/nk_button_symbol context Nuklear/NK_SYMBOL_UNDERSCORE)
         (if (Nuklear/nk_button_image context download-icon)
           (println "Download"))
         ; ...
         ))

; ...

(GL11/glDeleteTextures download-tex)

; ...
{% endhighlight %}
Here is a screenshot showing the buttons instantiated in the code above.
<span class="center"><img src="/pics/buttons.png" width="508" alt="Symbol and Image Buttons"/></span>

#### Combo Boxes

A combo box lets you choose an option using a drop down menu.
It is even possible to have combo boxes with multiple columns.
{% highlight clojure %}
; ...

(def combo-size (NkVec2/create))
(.x combo-size 320)
(.y combo-size 120)

; ...

(def combo-items (mapv #(str "test" (inc %)) (range 10)))
(def selected (atom (first combo-items)))

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         ; ...
         (Nuklear/nk_layout_row_dynamic context 32 1)
         (when (Nuklear/nk_combo_begin_label context @selected combo-size)
           (Nuklear/nk_layout_row_dynamic context 32 1)
           (doseq [item combo-items]
                  (if (Nuklear/nk_combo_item_text context item
                                                  Nuklear/NK_TEXT_LEFT)
                    (reset! selected item)))
           (Nuklear/nk_combo_end context))
         ; ...
         ))

; ...

(GL11/glDeleteTextures download-tex)

; ...
{% endhighlight %}
The combo box in the following screenshot uses one column.
<span class="center"><img src="/pics/combo.png" width="508" alt="Combo Box"/></span>

#### Drawing Custom Widgets

It is possible to use draw commands to draw a custom widget.
There are also methods for checking if the mouse is hovering over the widget or if the mouse was clicked.
{% highlight clojure %}
(ns nukleartest
    (:import ; ...
             [org.lwjgl.nuklear ; ...
              NkColor]
             ; ...
             ))

; ...

(def rgb (NkColor/malloc stack))

; ...

(while (not (GLFW/glfwWindowShouldClose window))
       ; ...
       (when (Nuklear/nk_begin context "Nuklear Example"
                               (Nuklear/nk_rect 0 0 width height rect) 0)
         ; ...
         (let [canvas (Nuklear/nk_window_get_canvas context)]
           (Nuklear/nk_layout_row_dynamic context 120 1)
           (if (Nuklear/nk_widget_is_mouse_clicked context Nuklear/NK_BUTTON_LEFT)
             (println "Widget clicked"))
           (let [color (if (Nuklear/nk_widget_is_hovered context)
                         (Nuklear/nk_rgb 255 255 255 rgb)
                         (Nuklear/nk_rgb 255 127 127 rgb))]
             (Nuklear/nk_widget rect context)
             (Nuklear/nk_fill_rect canvas rect 2 color)
             (Nuklear/nk_fill_circle canvas (Nuklear/nk_rect (+ (.x rect) (- (/ (.w rect) 2) 32))
                                                             (+ (.y rect) (- (/ (.h rect) 2) 32)) 64 64 rect)
                                     (Nuklear/nk_rgb 127 255 127 rgb))))
         ; ...
         ))

; ...
{% endhighlight %}
Here we have just drawn a filled rectangle and a filled circle.
<span class="center"><img src="/pics/widget.png" width="508" alt="Custom Widget"/></span>

[1]: https://www.lwjgl.org/
[2]: https://immediate-mode-ui.github.io/Nuklear/doc/index.html
[3]: https://github.com/LWJGL/lwjgl3/blob/master/modules/samples/src/test/java/org/lwjgl/demo/nuklear/GLFWDemo.java
[4]: https://github.com/wedesoft/nukleartest
[5]: https://www.wedesoft.de/software/2023/05/26/lwjgl3-clojure/
[6]: https://repo1.maven.org/maven2/org/lwjgl/lwjgl-opengl/3.3.2/
[7]: https://github.com/HumbleUI/HumbleUI
[8]: https://github.com/nothings/stb
[9]: http://quil.info/
