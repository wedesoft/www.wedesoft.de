---
layout: post
title: Implementing GUIs using Clojure and LWJGL Nuklear bindings
category: software
image: /pics/nuklear.png
---

<span class="center"><img src="/pics/nuklear.png" width="508" alt="Nuklear GUI"/></span>

For game development I have been using [LWJGL3][1] which is a great Java library for cross-platform development.
Among other things it has bindings for *OpenGL*, *GLFW*, and *STB*.
Recently I discovered the *Nuklear* bindings.
[Nuklear][2] is a small C library useful for developing GUIs for games.
It receives control input and commands to populate a GUI and converts those into render instructions.
Nuklear focuses solely on the user interface, while input and graphics backend are handled by the application.
It is therefore very flexible and can be integrated into a 3D game implemented using OpenGL, DirectX, Vulkan, or other.

LWJGL Nuklear bindings come with the [GLFWDemo.java][3] example.
In this article I have basically translated the input and graphics backend to Clojure.
I also added examples for several different controls.
I have [pushed the source code to Github][4] if you want to look at it straight away.

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
You can run the program using *clj -M -m nukleartest*.

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
The vertex shader passes through texture coordinates and fragment colors.
Furthermore it scales the input position to OpenGL normalized device coordinates (NDCs).
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

[1]: https://www.lwjgl.org/
[2]: https://immediate-mode-ui.github.io/Nuklear/doc/index.html
[3]: https://github.com/LWJGL/lwjgl3/blob/master/modules/samples/src/test/java/org/lwjgl/demo/nuklear/GLFWDemo.java
[4]: https://github.com/wedesoft/nukleartest
[5]: https://www.wedesoft.de/software/2023/05/26/lwjgl3-clojure/
