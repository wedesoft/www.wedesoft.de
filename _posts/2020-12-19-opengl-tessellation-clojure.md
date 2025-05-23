---
layout: post
title: Tessellation OpenGL example using Clojure and LWJGL
category: software
---

Last week I published a [minimal OpenGL example in Clojure][1].
The example was implemented using [LWJGL version 2][2] (see [LWJGL Wiki][3] for documentation).
My motivation is to implement a space flight game, which requires rendering of planetary bodies.
I found an impressive free software called the [Oreon Engine][4] which is [implemented in Java][5].
Watching the [videos][4] I realised that I should investigate tessellation shaders.
I didn't find any small single-page code examples for tessellation shaders, so I decided to publish one here.
The example is implemented in Clojure but could be easily ported to C if you prefer.

The example uses several shaders required when doing tessellation using OpenGL:

* a vertex shader
* a tessellation control shader
* a tessellation evaluation shader
* a geometry shader
* a fragment shader

The example not only shows how to set the tessellation level but it also shows how one can pass through texture coordinates.
The polygon mode was switched to display lines only so that one can observe how the triangle is split up.

See code below:

{% highlight clojure %}
(ns tessellation-opengl
  (:import [org.lwjgl BufferUtils]
           [org.lwjgl.opengl Display DisplayMode GL11 GL12 GL13 GL15 GL20 GL30 GL32 GL40]))

(def vertex-source "#version 410 core
in mediump vec3 point;
in mediump vec2 texcoord;
out mediump vec2 texcoord_tcs;
void main()
{
  gl_Position = vec4(point, 1);
  texcoord_tcs = texcoord;
}")

(def tcs-source "#version 410 core
layout(vertices = 4) out;
in mediump vec2 texcoord_tcs[];
out mediump vec2 texcoord_tes[];
void main(void)
{
  if (gl_InvocationID == 0) {
    gl_TessLevelOuter[0] = 2.0;
    gl_TessLevelOuter[1] = 3.0;
    gl_TessLevelOuter[2] = 4.0;
    gl_TessLevelOuter[3] = 5.0;
    gl_TessLevelInner[0] = 6.0;
    gl_TessLevelInner[1] = 7.0;
  }
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  texcoord_tes[gl_InvocationID] = texcoord_tcs[gl_InvocationID];
}")

(def tes-source "#version 410 core
layout(quads, equal_spacing, ccw) in;
in mediump vec2 texcoord_tes[];
out mediump vec2 texcoord_geo;
void main()
{
  vec4 a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
  vec4 b = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
  gl_Position = mix(a, b, gl_TessCoord.y);
  vec2 c = mix(texcoord_tes[0], texcoord_tes[1], gl_TessCoord.x);
  vec2 d = mix(texcoord_tes[3], texcoord_tes[2], gl_TessCoord.x);
  texcoord_geo = mix(c, d, gl_TessCoord.y);
}")

(def geo-source "#version 410 core
layout(triangles) in;
in mediump vec2 texcoord_geo[3];
layout(triangle_strip, max_vertices = 3) out;
out mediump vec2 UV;
void main(void)
{
  gl_Position = gl_in[0].gl_Position;
  UV = texcoord_geo[0];
  EmitVertex();
  gl_Position = gl_in[1].gl_Position;
  UV = texcoord_geo[1];
  EmitVertex();
  gl_Position = gl_in[2].gl_Position;
  UV = texcoord_geo[2];
  EmitVertex();
  EndPrimitive();
}")

(def fragment-source "#version 410 core
in mediump vec2 UV;
out mediump vec3 fragColor;
uniform sampler2D tex;
void main()
{
  fragColor = texture(tex, UV).rgb;
}")

(def vertices
  (float-array [ 0.5  0.5 0.0 1.0 1.0
                -0.5  0.5 0.0 0.0 1.0
                -0.5 -0.5 0.0 0.0 0.0
                 0.5 -0.5 0.0 1.0 0.0]))

(def indices
  (int-array [0 1 2 3]))

(def pixels
  (float-array [0.0 0.0 1.0
                0.0 1.0 0.0
                1.0 0.0 0.0
                1.0 1.0 1.0]))

(defn make-shader [source shader-type]
  (let [shader (GL20/glCreateShader shader-type)]
    (GL20/glShaderSource shader source)
    (GL20/glCompileShader shader)
    (if (zero? (GL20/glGetShaderi shader GL20/GL_COMPILE_STATUS))
      (throw (Exception. (GL20/glGetShaderInfoLog shader 1024))))
    shader))

(defn make-program [& shaders]
  (let [program (GL20/glCreateProgram)]
    (doseq [shader shaders] (GL20/glAttachShader program shader))
    (GL20/glLinkProgram program)
    (if (zero? (GL20/glGetProgrami program GL20/GL_LINK_STATUS))
      (throw (Exception. (GL20/glGetProgramInfoLog program 1024))))
    program))

(defmacro def-make-buffer [method create-buffer]
  `(defn ~method [data#]
     (let [buffer# (~create-buffer (count data#))]
       (.put buffer# data#)
       (.flip buffer#)
       buffer#)))

(def-make-buffer make-float-buffer BufferUtils/createFloatBuffer)
(def-make-buffer make-int-buffer BufferUtils/createIntBuffer)

(Display/setTitle "mini")
(Display/setDisplayMode (DisplayMode. 320 240))
(Display/create)

(def vertex-shader (make-shader vertex-source GL20/GL_VERTEX_SHADER))
(def fragment-shader (make-shader fragment-source GL20/GL_FRAGMENT_SHADER))
(def tcs-shader (make-shader tcs-source GL40/GL_TESS_CONTROL_SHADER))
(def tes-shader (make-shader tes-source GL40/GL_TESS_EVALUATION_SHADER))
(def geo-shader (make-shader geo-source GL32/GL_GEOMETRY_SHADER))
(def program (make-program vertex-shader fragment-shader geo-shader tcs-shader tes-shader))

(def vao (GL30/glGenVertexArrays))
(GL30/glBindVertexArray vao)

(def vbo (GL15/glGenBuffers))
(GL15/glBindBuffer GL15/GL_ARRAY_BUFFER vbo)
(def vertices-buffer (make-float-buffer vertices))
(GL15/glBufferData GL15/GL_ARRAY_BUFFER vertices-buffer GL15/GL_STATIC_DRAW)

(def idx (GL15/glGenBuffers))
(GL15/glBindBuffer GL15/GL_ELEMENT_ARRAY_BUFFER idx)
(def indices-buffer (make-int-buffer indices))
(GL15/glBufferData GL15/GL_ELEMENT_ARRAY_BUFFER indices-buffer GL15/GL_STATIC_DRAW)

(GL20/glVertexAttribPointer (GL20/glGetAttribLocation program "point"   ) 3 GL11/GL_FLOAT false (* 5 Float/BYTES) (* 0 Float/BYTES))
(GL20/glVertexAttribPointer (GL20/glGetAttribLocation program "texcoord") 2 GL11/GL_FLOAT false (* 5 Float/BYTES) (* 3 Float/BYTES))
(GL20/glEnableVertexAttribArray 0)
(GL20/glEnableVertexAttribArray 1)

(GL20/glUseProgram program)

(def tex (GL11/glGenTextures))
(GL13/glActiveTexture GL13/GL_TEXTURE0)
(GL11/glBindTexture GL11/GL_TEXTURE_2D tex)
(GL20/glUniform1i (GL20/glGetUniformLocation program "tex") 0)
(def pixel-buffer (make-float-buffer pixels))
(GL11/glTexImage2D GL11/GL_TEXTURE_2D 0 GL11/GL_RGB 2 2 0 GL12/GL_BGR GL11/GL_FLOAT pixel-buffer)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_WRAP_S GL12/GL_CLAMP_TO_EDGE)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_WRAP_T GL12/GL_CLAMP_TO_EDGE)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MIN_FILTER GL11/GL_NEAREST)
(GL11/glTexParameteri GL11/GL_TEXTURE_2D GL11/GL_TEXTURE_MAG_FILTER GL11/GL_NEAREST)
(GL30/glGenerateMipmap GL11/GL_TEXTURE_2D)

(GL11/glPolygonMode GL11/GL_FRONT_AND_BACK GL11/GL_LINE)

(while (not (Display/isCloseRequested))
  (GL11/glClearColor 0.0 0.0 0.0 0.0)
  (GL11/glClear (bit-or GL11/GL_COLOR_BUFFER_BIT GL11/GL_DEPTH_BUFFER_BIT))
  (GL40/glPatchParameteri GL40/GL_PATCH_VERTICES 4)
  (GL11/glDrawElements GL40/GL_PATCHES 4 GL11/GL_UNSIGNED_INT 0)
  (Display/update)
  (Thread/sleep 40))

(GL20/glDisableVertexAttribArray 1)
(GL20/glDisableVertexAttribArray 0)

(GL11/glBindTexture GL11/GL_TEXTURE_2D 0)
(GL11/glDeleteTextures tex)

(GL15/glBindBuffer GL15/GL_ELEMENT_ARRAY_BUFFER 0)
(GL15/glDeleteBuffers idx)

(GL15/glBindBuffer GL15/GL_ARRAY_BUFFER 0)
(GL15/glDeleteBuffers vbo)

(GL30/glBindVertexArray 0)
(GL30/glDeleteVertexArrays vao)

(GL20/glDetachShader program vertex-shader)
(GL20/glDetachShader program fragment-shader)
(GL20/glDeleteProgram program)
(GL20/glDeleteShader vertex-shader)
(GL20/glDeleteShader fragment-shader)

(Display/destroy)
{% endhighlight %}

![image](/pics/square.png)

Any feedback, comments, and suggestions are welcome.

Enjoy!

**Update:**

If you are using Clojure 1.11 or later, you need to use a `deps.edn` file or specify the dependency on the command line like this:

{% highlight shell %}
wget https://www.wedesoft.de/downloads/tessellation-opengl.clj
sudo apt-get install liblwjgl-java
clj -Sdeps '{:deps {lwglj/lwglj {:local/root "/usr/share/java/lwjgl.jar"}}}' -M tessellation-opengl.clj
{% endhighlight %}

[1]: https://www.wedesoft.de/software/2020/12/10/opengl-clojure/
[2]: http://legacy.lwjgl.org/
[3]: http://wiki.lwjgl.org/
[4]: https://www.youtube.com/channel/UC9lXX-YnU_VcDT3VS85skMQ
[5]: https://github.com/fynnfluegge/oreon-engine
