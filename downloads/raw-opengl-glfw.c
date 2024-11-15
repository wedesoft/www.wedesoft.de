// Minimal OpenGL example using GLFW and GLEW
#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Vertex shader source code:
// This shader takes in vertex positions and texture coordinates,
// passing them to the fragment shader.
const char *vertexSource = "#version 130\n\
in mediump vec3 point;\n\
in mediump vec2 texcoord;\n\
out mediump vec2 UV;\n\
void main()\n\
{\n\
  gl_Position = vec4(point, 1);\n\
  UV = texcoord;\n\
}";

// Fragment shader source code:
// This shader samples the color from a texture based on UV coordinates.
const char *fragmentSource = "#version 130\n\
in mediump vec2 UV;\n\
out mediump vec3 fragColor;\n\
uniform sampler2D tex;\n\
void main()\n\
{\n\
  fragColor = texture(tex, UV).rgb;\n\
}";

GLuint vao; // Vertex Array Object
GLuint vbo; // Vertex Buffer Object
GLuint idx; // Index Buffer Object
GLuint tex; // Texture
GLuint program; // Shader program
int width = 320; // Width of window in pixels
int height = 240; // Height of window in pixels

// Function to handle shader compile errors
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

// Function to handle shader program link errors
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

// Vertex data:
// Each vertex has a position (x, y, z) and a texture coordinate (u, v)
GLfloat vertices[] = {
   0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Top right
  -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // Top left
  -0.5f, -0.5f,  0.0f, 0.0f, 0.0f   // Bottom left
};

// Indices for drawing the triangle
unsigned int indices[] = { 0, 1, 2 };

// Texture BGR data for a 2x2 texture
float pixels[] = {
  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

int main(int argc, char** argv)
{
  // Initialize GLFW library.
  glfwInit();
  // Create a window.
  GLFWwindow *window = glfwCreateWindow(width, height, "minimal OpenGL example", NULL, NULL);
  // Set current OpenGL context to window.
  glfwMakeContextCurrent(window);
  // Initialize GLEW library.
  glewInit();

  glViewport(0, 0, width, height);

  // Compile and check vertex shader.
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  handleCompileError("Vertex shader", vertexShader);

  // Compile and check fragment shader.
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  handleCompileError("Fragment shader", fragmentShader);

  // Link and check shader program.
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  handleLinkError("Shader program", program);

  // Create a vertex array object which serves as context for the
  // vertex buffer object and the index buffer object.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Initialize vertex buffer object with the vertex data.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Initialize the index buffer object with the index data.
  glGenBuffers(1, &idx);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set up layout of vertex buffer object.
  glVertexAttribPointer(glGetAttribLocation(program, "point"), 3, GL_FLOAT,
                        GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(glGetAttribLocation(program, "texcoord"), 2, GL_FLOAT,
                        GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

  // Enable depth testing using depth buffer.
  glEnable(GL_DEPTH_TEST);

  // Switch to the shader program.
  glUseProgram(program);

  // Enable the two variables of the vertex buffer layout.
  glEnableVertexAttribArray(glGetAttribLocation(program, "point"));
  glEnableVertexAttribArray(glGetAttribLocation(program, "texcoord"));

  // Initialize texture.
  glGenTextures(1, &tex);
  // Bind texture to first slot.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  // Set uniform texture in shader object to first texture.
  glUniform1i(glGetUniformLocation(program, "tex"), 0);
  // Load pixel data into texture.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_BGR, GL_FLOAT, pixels);
  // Set texture wrapping mode and interpolation modes.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // Initialize multiresolution layers.
  glGenerateMipmap(GL_TEXTURE_2D);

  // Loop until the user closes the window.
  while (!glfwWindowShouldClose(window)) {
    // Clear color buffer and depth buffer.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // Switch to the shader program.
    glUseProgram(program);
    // Draw triangle(s).
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)0);
    // Swap front and back buffers.
    glfwSwapBuffers(window);
    // Poll for and process events.
    glfwPollEvents();
  };

  // Disable the two shader variables.
  glDisableVertexAttribArray(glGetAttribLocation(program, "point"));
  glDisableVertexAttribArray(glGetAttribLocation(program, "texcoord"));

  // Unbind and delete the texture.
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &tex);

  // Unbind and delete the index buffer object.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &idx);

  // Unbind and delete the vertex buffer object.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);

  // Unbind and delete the vertex array object.
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  // Unlink and delete the shader program.
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Set OpenGL context to NULL.
  glfwMakeContextCurrent(NULL);
  // Destroy window.
  glfwDestroyWindow(window);
  // Terminate GLFW.
  glfwTerminate();
  return 0;
}
