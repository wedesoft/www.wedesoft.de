---
layout: post
title: Minimal OpenGL example in C using GLEW and GLFW
category: software
image: /pics/triangle.png
---

OpenGL is a reasonably abstract API for doing 3D graphics.
In the past I did an [example of OpenGL using GLUT][1].
However GLUT is a bit outdated now and a more modern alternative is [GLFW][2].
The example still uses [GLEW][3] to setup the OpenGL extensions.

This example is minimal and only uses a vertex shader and a fragment shader to get started with OpenGL.
For an example using tesselation and geometry shaders as well, see my [short introduction to OpenGL][4].

Note that it is important to add code for retrieving error messages (as I have done below) in order to be able to do development of the shaders.

As in my old example, the code draws a coloured triangle on the screen.

{% highlight c %}
// Minimal OpenGL example using GLFW and GLEW
#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


const char *vertexSource = "#version 130\n\
in mediump vec3 point;\n\
in mediump vec2 texcoord;\n\
out mediump vec2 UV;\n\
void main()\n\
{\n\
  gl_Position = vec4(point, 1);\n\
  UV = texcoord;\n\
}";

const char *fragmentSource = "#version 130\n\
in mediump vec2 UV;\n\
out mediump vec3 fragColor;\n\
uniform sampler2D tex;\n\
void main()\n\
{\n\
  fragColor = texture(tex, UV).rgb;\n\
}";

GLuint vao;
GLuint vbo;
GLuint idx;
GLuint tex;
GLuint program;
int width = 320;
int height = 240;

void handleCompileError(const char *step, GLuint shader)
{
  GLint result = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    char buffer[1024];
    glGetShaderInfoLog(shader, 1024, NULL, buffer);
    if (buffer[0])
      fprintf(stderr, "%s: %s\n", step, buffer);
  };
}

void handleLinkError(const char *step, GLuint program)
{
  GLint result = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (result == GL_FALSE) {
    char buffer[1024];
    glGetProgramInfoLog(program, 1024, NULL, buffer);
    if (buffer[0])
      fprintf(stderr, "%s: %s\n", step, buffer);
  };
}

GLfloat vertices[] = {
   0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
  -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f,  0.0f, 0.0f, 0.0f
};

unsigned int indices[] = { 0, 1, 2 };

float pixels[] = {
  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

int main(int argc, char** argv)
{
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(width, height, "minimal OpenGL example", NULL, NULL);
  glfwMakeContextCurrent(window);
  glewInit();

  glViewport(0, 0, width, height);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  handleCompileError("Vertex shader", vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  handleCompileError("Fragment shader", fragmentShader);

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  handleLinkError("Shader program", program);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &idx);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(glGetAttribLocation(program, "point"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(glGetAttribLocation(program, "texcoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

  glEnable(GL_DEPTH_TEST);

  glUseProgram(program);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(program, "tex"), 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_BGR, GL_FLOAT, pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  };

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &tex);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &idx);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
{% endhighlight %}

The example uses the widely supported OpenGL version 3.1 (which has the version tag 130).
You can download, compile, and run the example as follows:

{% highlight shell %}
wget https://www.wedesoft.de/downloads/raw-opengl-glfw.c
gcc -o raw-opengl-glfw raw-opengl-glfw.c $(pkg-config --libs glfw3 glew)
./raw-opengl-glfw
{% endhighlight %}

![image](/pics/triangle.png)

Any feedback, comments, and suggestions are welcome.

Enjoy!

[1]: https://www.wedesoft.de/software/2018/08/03/raw-opengl/
[2]: https://www.glfw.org/
[3]: https://glew.sourceforge.net/
[4]: https://www.wedesoft.de/software/2024/05/30/short-opengl-intro/
