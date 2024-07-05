#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int width = 640;
int height = 480;

const char *vertexSource = "#version 410 core\n\
in vec3 point;\n\
in vec2 texcoord;\n\
out vec2 uv_vert;\n\
void main()\n\
{\n\
  gl_Position = vec4(point, 1);\n\
  uv_vert = texcoord;\n\
}";

const char *tessControlSource = "#version 410 core\n\
layout(vertices = 4) out;\n\
in vec2 uv_vert[];\n\
out vec2 uv_contr[];\n\
void main()\n\
{\n\
  if (gl_InvocationID == 0) {\n\
    gl_TessLevelOuter[0] = 25;\n\
    gl_TessLevelOuter[1] = 25;\n\
    gl_TessLevelOuter[2] = 25;\n\
    gl_TessLevelOuter[3] = 25;\n\
    gl_TessLevelInner[0] = 25;\n\
    gl_TessLevelInner[1] = 25;\n\
  };\n\
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n\
  uv_contr[gl_InvocationID] = uv_vert[gl_InvocationID];\n\
}";

const char *tessEvalSource = "#version 410 core\n\
layout(quads, equal_spacing, ccw) in;\n\
uniform mat3 rotz;\n\
uniform mat3 rotx;\n\
uniform mat4 projection;\n\
uniform float distance;\n\
in vec2 uv_contr[];\n\
out vec2 uv_eval;\n\
out vec3 normal_eval;\n\
float amplitude = 0.4;\n\
float scale = 30;\n\
float sinc(float x)\n\
{\n\
  return x > 0 ? sin(x) / x : 1.0;\n\
}\n\
float f(vec2 v)\n\
{\n\
  return amplitude * sinc(scale * length(v));\n\
}\n\
vec2 fdv(vec2 v)\n\
{\n\
  float l = length(v);\n\
  if (l > 0) {\n\
    float radial = (cos(scale * l) / (l * l) - sin(scale * l) / (scale * (l * l * l)));\n\
    return amplitude * v * radial;\n\
  } else\n\
    return vec2(0, 0);\n\
}\n\
void main()\n\
{\n\
  vec4 pos = mix(mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x),\n\
                 mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x),\n\
                 gl_TessCoord.y);\n\
  pos.z = f(pos.xy);\n\
  normal_eval = rotx * rotz * vec3(-fdv(pos.xy), 1);\n\
  vec3 translation = vec3(0, 0, -distance);\n\
  gl_Position = projection * vec4(rotx * rotz * pos.xyz + translation, 1);\n\
  uv_eval = mix(mix(uv_contr[0], uv_contr[1], gl_TessCoord.x),\n\
                mix(uv_contr[3], uv_contr[2], gl_TessCoord.x),\n\
                gl_TessCoord.y);\n\
}";

const char *geometrySource = "#version 410 core\n\
layout(triangles) in;\n\
in vec2 uv_eval[3];\n\
in vec3 normal_eval[3];\n\
layout(triangle_strip, max_vertices = 3) out;\n\
out vec2 UV;\n\
out vec3 normal;\n\
void main(void)\n\
{\n\
  gl_Position = gl_in[0].gl_Position;\n\
  UV = uv_eval[0];\n\
  normal = normal_eval[0];\n\
  EmitVertex();\n\
  gl_Position = gl_in[1].gl_Position;\n\
  UV = uv_eval[1];\n\
  normal = normal_eval[1];\n\
  EmitVertex();\n\
  gl_Position = gl_in[2].gl_Position;\n\
  UV = uv_eval[2];\n\
  normal = normal_eval[2];\n\
  EmitVertex();\n\
  EndPrimitive();\n\
}";

const char *fragmentSource = "#version 410 core\n\
uniform sampler2D tex;\n\
uniform vec3 light;\n\
in vec2 UV;\n\
in vec3 normal;\n\
out vec3 fragColor;\n\
void main()\n\
{\n\
  vec3 n = normalize(normal);\n\
  float ambient = 0.2;\n\
  float diffuse = 0.8 * max(dot(light, n), 0);\n\
  fragColor = (ambient + diffuse) * texture(tex, UV).rgb;\n\
}";

GLuint vao;
GLuint vbo;
GLuint idx;

GLfloat vertices[] = {
  -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
   0.5f, -0.5f,  0.0f, 6.0f, 0.0f,
  -0.5f,  0.5f,  0.0f, 0.0f, 6.0f,
   0.5f,  0.5f,  0.0f, 6.0f, 6.0f
};

unsigned int indices[] = {0, 1, 3, 2};

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

float chequer[] = {
  0.4f, 0.4f, 0.4f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.4f
};

void main(void)
{
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL example", NULL, NULL);
  glfwMakeContextCurrent(window);
  glewInit();

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  glViewport(0, 0, width, height);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  handleCompileError("Vertex shader", vertexShader);

  GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
  glShaderSource(tessControlShader, 1, &tessControlSource, NULL);
  glCompileShader(tessControlShader);
  handleCompileError("Tessellation Control shader", tessControlShader);

  GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
  glShaderSource(tessEvalShader, 1, &tessEvalSource, NULL);
  glCompileShader(tessEvalShader);
  handleCompileError("Tessellation Evaluation shader", tessEvalShader);

  GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShader, 1, &geometrySource, NULL);
  glCompileShader(geometryShader);
  handleCompileError("Geometry shader", geometryShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  handleCompileError("Fragment shader", fragmentShader);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, tessControlShader);
  glAttachShader(program, tessEvalShader);
  glAttachShader(program, geometryShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  handleLinkError("Shader program", program);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);
  glGenBuffers(1, &idx);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(glGetAttribLocation(program, "point"),
                        3, GL_FLOAT, GL_FALSE,
                        5 * sizeof(float), (void *)0);
  glVertexAttribPointer(glGetAttribLocation(program, "texcoord"),
                        2, GL_FLOAT, GL_FALSE,
                        5 * sizeof(float), (void *)(3 * sizeof(float)));

  glUseProgram(program);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  GLuint tex;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(program, "tex"), 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_BGR, GL_FLOAT, chequer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glDepthFunc(GL_GEQUAL);
  glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
  glClearDepth(0.0);
  glEnable(GL_DEPTH_TEST);

  float alpha = 30 * M_PI / 180;
  float ca = cos(alpha);
  float sa = sin(alpha);
  float rotz[9] = {ca, sa, 0, -sa, ca, 0, 0, 0, 1};
  glUniformMatrix3fv(glGetUniformLocation(program, "rotz"), 1, GL_TRUE, rotz);

  float beta = 60 * M_PI / 180;
  float cb = cos(beta);
  float sb = sin(beta);
  float rotx[9] = {1, 0, 0, 0, cb, sb, 0, -sb, cb};
  glUniformMatrix3fv(glGetUniformLocation(program, "rotx"), 1, GL_TRUE, rotx);

  glUniform1f(glGetUniformLocation(program, "distance"), 1.8);

  float fov = 45.0 * M_PI / 180;
  float near = 0.1;
  float far = 10.0;
  float dx = 1.0 / tan(0.5 * fov);
  float dy = dx * width / height;
  float a = far * near / (far - near);
  float b = near / (far - near);
  float projection[16] = {dx, 0, 0, 0, 0, dy, 0, 0, 0, 0, b, a, 0, 0, -1, 0};
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projection);

  float light[3] = {sqrt(0.5), sqrt(0.5), 0.0};
  glUniform3fv(glGetUniformLocation(program, "light"), 1, light);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_INT, (void *)0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  };

  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &tex);

  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &idx);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(tessControlShader);
  glDeleteShader(tessEvalShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  glfwTerminate();
}
