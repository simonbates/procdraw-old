#include "procdraw/graphics/glrenderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "procdraw/graphics/gl_util.h"
#include "procdraw/graphics/sdl_util.h"
#include "procdraw/utils/color.h"
#include "procdraw/utils/utils.h"
#include <iostream>
#include <string>

namespace procdraw {

GlRenderer::GlRenderer()
  : window_(nullptr)
  , glcontext_(NULL)
  , program_(0)
  , tetrahedronVertexBuffer_(0)
  , tetrahedronVao_(0)
  , cubeIndexBuffer_(0)
  , cubeVertexBuffer_(0)
  , cubeVao_(0)
{
  CreateWindowAndGlContext();

  CompileShaders();
  CompileShaders2d();

  MakeTetrahedronVao();
  MakeCubeVao();
  MakeRectangleVao();

  ResetMatrix();
  InitLighting();
  InitMaterial();

  // Create the text renderer after we have initialized OpenGL
  textRenderer_ = std::unique_ptr<FtTextRenderer>(new FtTextRenderer());

  Begin3D();
}

GlRenderer::~GlRenderer()
{
  // TODO The constructor/destructor design here is not safe -- if the
  // constructor throws an exception, the destructor will not be called

  glDeleteVertexArrays(1, &tetrahedronVao_);
  glDeleteBuffers(1, &tetrahedronVertexBuffer_);

  glDeleteVertexArrays(1, &cubeVao_);
  glDeleteBuffers(1, &cubeVertexBuffer_);
  glDeleteBuffers(1, &cubeIndexBuffer_);

  glDeleteVertexArrays(1, &rectangleVao_);
  glDeleteBuffers(1, &rectangleVertexBuffer_);

  glDeleteProgram(program_);
  glDeleteProgram(program2d_);

  if (glcontext_ != NULL) {
    SDL_GL_DeleteContext(glcontext_);
  }

  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
  }
}

void
GlRenderer::AmbientLightColor(float h, float s, float v)
{
  float r, g, b;
  Hsv2Rgb(h, s, v, r, g, b);
  ambientLightColor_.x = r;
  ambientLightColor_.y = g;
  ambientLightColor_.z = b;
  ambientLightColor_.w = 0.0f;
}

void
GlRenderer::Background(float h, float s, float v)
{
  float r, g, b;
  Hsv2Rgb(h, s, v, r, g, b);
  glClearColor(r, g, b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ResetMatrix();
}

void
GlRenderer::Begin2D()
{
  glUseProgram(program2d_);
  // TODO Cache the 2d projection matrix -- no need to calculate
  // each time, only when the renderer size changes
  auto projection = glm::ortho(0.0f, static_cast<float>(Width()),
                               static_cast<float>(Height()), 0.0f);
  glUniformMatrix4fv(projection2dLoc_, 1, GL_FALSE, glm::value_ptr(projection));
  glDisable(GL_DEPTH_TEST);
}

void
GlRenderer::Begin3D()
{
  glUseProgram(program_);
  glEnable(GL_DEPTH_TEST);
}

void
GlRenderer::BeginInverse()
{
  Color(0, 0, 1.0f, 1.0f);
  glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
}

void
GlRenderer::BeginText()
{
  // TODO: Passing the width and height doesn't feel like a great solution
  textRenderer_->BeginText(Width(), Height());
}

// TODO: ColorHsv and ColorRgb

void
GlRenderer::Color(float h, float s, float v, float a)
{
  Hsv2Rgb(h, s, v, materialR_, materialG_, materialB_);
  materialA_ = a;
}

void
GlRenderer::Cube()
{
  UpdateUniformsForObject();
  glBindVertexArray(cubeVao_);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void
GlRenderer::DoSwap()
{
  SDL_GL_SwapWindow(window_);
}

void
GlRenderer::DrawBlockCursorBackground(int cursorX, int cursorY, int cursorWidth,
                                      int cursorHeight)
{
  Color(0, 0, 1.0f / 32.0f, 1.0f);
  Rect(cursorX, cursorY, cursorWidth, cursorHeight);
}

void
GlRenderer::DrawBlockCursorInversion(int cursorX, int cursorY, int cursorWidth,
                                     int cursorHeight)
{
  BeginInverse();
  Rect(cursorX, cursorY, cursorWidth, cursorHeight);
  EndInverse();
}

void
GlRenderer::DrawText(int x, int y, const TextLayout<GLfloat>& layout,
                     TextLayout<GLfloat>::size_type startLineNum,
                     TextLayout<GLfloat>::size_type endLineNum)
{
  textRenderer_->DrawText(x, y, layout, startLineNum, endLineNum);
}

void
GlRenderer::EndInverse()
{
  SetDefaultBlend();
}

int
GlRenderer::GetLinespace()
{
  return textRenderer_->GetLinespace();
}

int
GlRenderer::Height()
{
  int w, h;
  SDL_GetWindowSize(window_, &w, &h);
  return h;
}

TextLayout<GLfloat>
GlRenderer::LayOutText(const std::string& text, int maxLineWidthPixels)
{
  return textRenderer_->LayOutText(text, maxLineWidthPixels);
}

void
GlRenderer::LightColor(float h, float s, float v)
{
  float r, g, b;
  Hsv2Rgb(h, s, v, r, g, b);
  lightColor_.x = r;
  lightColor_.y = g;
  lightColor_.z = b;
  lightColor_.w = 0.0f;
}

double
GlRenderer::MouseX()
{
  int x, y;
  SDL_GetMouseState(&x, &y);
  return static_cast<double>(x) / (Width() - 1);
}

double
GlRenderer::MouseY()
{
  int x, y;
  SDL_GetMouseState(&x, &y);
  return static_cast<double>(y) / (Height() - 1);
}

void
GlRenderer::Rect(int x, int y, int w, int h)
{
  rectangleVertices_[0] = x;
  rectangleVertices_[1] = y;
  rectangleVertices_[2] = x;
  rectangleVertices_[3] = y + h;
  rectangleVertices_[4] = x + w;
  rectangleVertices_[5] = y;
  rectangleVertices_[6] = x + w;
  rectangleVertices_[7] = y + h;

  glUniform4f(materialColor2dLoc_, materialR_, materialG_, materialB_,
              materialA_);
  glBindVertexArray(rectangleVao_);
  glBindBuffer(GL_ARRAY_BUFFER, rectangleVertexBuffer_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rectangleVertices_),
                  rectangleVertices_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
GlRenderer::RotateX(float turns)
{
  worldMatrix_ = glm::rotate(worldMatrix_, turns * 2 * static_cast<float>(M_PI),
                             glm::vec3(1.0f, 0.0f, 0.0f));
}

void
GlRenderer::RotateY(float turns)
{
  worldMatrix_ = glm::rotate(worldMatrix_, turns * 2 * static_cast<float>(M_PI),
                             glm::vec3(0.0f, 1.0f, 0.0f));
}

void
GlRenderer::RotateZ(float turns)
{
  worldMatrix_ = glm::rotate(worldMatrix_, turns * 2 * static_cast<float>(M_PI),
                             glm::vec3(0.0f, 0.0f, 1.0f));
}

void
GlRenderer::Scale(float factor)
{
  worldMatrix_ = glm::scale(worldMatrix_, glm::vec3(factor, factor, factor));
}

void
GlRenderer::Tetrahedron()
{
  UpdateUniformsForObject();
  glBindVertexArray(tetrahedronVao_);
  glDrawArrays(GL_TRIANGLES, 0, 12);
}

void
GlRenderer::Translate(float x, float y, float z)
{
  worldMatrix_ = glm::translate(worldMatrix_, glm::vec3(x, y, z));
}

int
GlRenderer::Width()
{
  int w, h;
  SDL_GetWindowSize(window_, &w, &h);
  return w;
}

void
GlRenderer::CreateWindowAndGlContext()
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  window_ =
    SDL_CreateWindow("Procdraw", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 720, 720, SDL_WINDOW_OPENGL);
  if (window_ == nullptr) {
    ThrowSdlError();
  }

  glcontext_ = SDL_GL_CreateContext(window_);
  if (glcontext_ == NULL) {
    ThrowSdlError();
  }

  // Turn on VSYNC
  if (SDL_GL_SetSwapInterval(1)) {
    ThrowSdlError();
  }

  if (gl3wInit()) {
    throw std::runtime_error("Failed to initialize OpenGL");
  }

  std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  SetDefaultBlend();
}

void
GlRenderer::SetDefaultBlend()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
GlRenderer::CompileShaders()
{
  static const GLchar* vertexShaderSource[] = {
    "#version 150                                                              "
    " \n"
    "uniform mat4 world_view_projection;                                       "
    " \n"
    "uniform vec4 light_direction;                                             "
    " \n"
    "uniform vec4 light_color;                                                 "
    " \n"
    "uniform vec4 ambient_light_color;                                         "
    " \n"
    "uniform vec4 material_color;                                              "
    " \n"
    "in vec4 position;                                                         "
    " \n"
    "in vec3 normal;                                                           "
    " \n"
    "out vec4 vs_color;                                                        "
    " \n"
    "void main(void)                                                           "
    " \n"
    "{                                                                         "
    " \n"
    "    gl_Position = world_view_projection * position;                       "
    " \n"
    "    float diff_factor = max(dot(normal, light_direction.xyz), 0);         "
    " \n"
    "    vec4 diff_component = (light_color * material_color * diff_factor);   "
    " \n"
    "    vec4 ambient_component = (ambient_light_color * material_color);      "
    " \n"
    "    vs_color.rgb = diff_component.rgb + ambient_component.rgb;            "
    " \n"
    "    vs_color.a = material_color.a;                                        "
    " \n"
    "}                                                                         "
    " \n"
  };

  static const GLchar* fragmentShaderSource[] = {
    "#version 150                                       \n"
    "in vec4 vs_color;                                  \n"
    "out vec4 color;                                    \n"
    "void main(void)                                    \n"
    "{                                                  \n"
    "    color = vs_color;                              \n"
    "}                                                  \n"
  };

  program_ = CompileProgram(vertexShaderSource, fragmentShaderSource,
                            { { 0, "position" }, { 1, "normal" } });

  worldViewProjectionLoc_ =
    glGetUniformLocation(program_, "world_view_projection");
  lightDirectionLoc_ = glGetUniformLocation(program_, "light_direction");
  lightColorLoc_ = glGetUniformLocation(program_, "light_color");
  ambientLightColorLoc_ = glGetUniformLocation(program_, "ambient_light_color");
  materialColorLoc_ = glGetUniformLocation(program_, "material_color");
}

void
GlRenderer::CompileShaders2d()
{
  static const GLchar* vertexShaderSource[] = {
    "#version 150                                                              "
    " \n"
    "uniform mat4 projection;                                                  "
    " \n"
    "in vec2 position;                                                         "
    " \n"
    "void main(void)                                                           "
    " \n"
    "{                                                                         "
    " \n"
    "    gl_Position = projection * vec4(position.xy, 0, 1);                   "
    " \n"
    "}                                                                         "
    " \n"
  };

  static const GLchar* fragmentShaderSource[] = {
    "#version 150                                       \n"
    "uniform vec4 material_color;                       \n"
    "out vec4 color;                                    \n"
    "void main(void)                                    \n"
    "{                                                  \n"
    "    color = material_color;                        \n"
    "}                                                  \n"
  };

  program2d_ = CompileProgram(vertexShaderSource, fragmentShaderSource,
                              { { 0, "position" } });
  projection2dLoc_ = glGetUniformLocation(program2d_, "projection");
  materialColor2dLoc_ = glGetUniformLocation(program2d_, "material_color");
}

void
GlRenderer::MakeTetrahedronVao()
{
  auto p1 = glm::vec3(1.0f, 0.0f, M_SQRT1_2);
  auto p2 = glm::vec3(0.0f, 1.0f, -M_SQRT1_2);
  auto p3 = glm::vec3(0.0f, -1.0f, -M_SQRT1_2);
  auto p4 = glm::vec3(-1.0f, 0.0f, M_SQRT1_2);

  auto n1 = TriangleNormal(p1, p2, p4);
  auto n2 = TriangleNormal(p1, p4, p3);
  auto n3 = TriangleNormal(p1, p3, p2);
  auto n4 = TriangleNormal(p2, p3, p4);

  Vertex vertices[] = { Vertex(p1, n1), Vertex(p2, n1), Vertex(p4, n1),
                        Vertex(p1, n2), Vertex(p4, n2), Vertex(p3, n2),
                        Vertex(p1, n3), Vertex(p3, n3), Vertex(p2, n3),
                        Vertex(p2, n4), Vertex(p3, n4), Vertex(p4, n4) };

  glGenVertexArrays(1, &tetrahedronVao_);
  glBindVertexArray(tetrahedronVao_);

  glGenBuffers(1, &tetrahedronVertexBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER, tetrahedronVertexBuffer_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, x));
  glEnableVertexAttribArray(0);

  // Normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, nx));
  glEnableVertexAttribArray(1);
}

void
GlRenderer::MakeCubeVao()
{
  auto p1 = glm::vec3(1.0f, 1.0f, 1.0f);
  auto p2 = glm::vec3(1.0f, 1.0f, -1.0f);
  auto p3 = glm::vec3(-1.0f, 1.0f, -1.0f);
  auto p4 = glm::vec3(-1.0f, 1.0f, 1.0f);
  auto p5 = glm::vec3(1.0f, -1.0f, 1.0f);
  auto p6 = glm::vec3(1.0f, -1.0f, -1.0f);
  auto p7 = glm::vec3(-1.0f, -1.0f, -1.0f);
  auto p8 = glm::vec3(-1.0f, -1.0f, 1.0f);

  auto n1 = TriangleNormal(p1, p4, p8);
  auto n2 = TriangleNormal(p1, p5, p6);
  auto n3 = TriangleNormal(p2, p6, p7);
  auto n4 = TriangleNormal(p3, p7, p8);
  auto n5 = TriangleNormal(p1, p2, p3);
  auto n6 = TriangleNormal(p5, p8, p7);

  Vertex vertices[] = { Vertex(p1, n1), Vertex(p4, n1), Vertex(p8, n1),
                        Vertex(p5, n1), Vertex(p1, n2), Vertex(p5, n2),
                        Vertex(p6, n2), Vertex(p2, n2), Vertex(p2, n3),
                        Vertex(p6, n3), Vertex(p7, n3), Vertex(p3, n3),
                        Vertex(p3, n4), Vertex(p7, n4), Vertex(p8, n4),
                        Vertex(p4, n4), Vertex(p1, n5), Vertex(p2, n5),
                        Vertex(p3, n5), Vertex(p4, n5), Vertex(p5, n6),
                        Vertex(p8, n6), Vertex(p7, n6), Vertex(p6, n6) };

  GLushort indices[] = { 0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
                         8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
                         16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };

  glGenVertexArrays(1, &cubeVao_);
  glBindVertexArray(cubeVao_);

  // Bind vertex buffer
  glGenBuffers(1, &cubeVertexBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, x));
  glEnableVertexAttribArray(0);

  // Normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, nx));
  glEnableVertexAttribArray(1);

  // Indices
  glGenBuffers(1, &cubeIndexBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBuffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

void
GlRenderer::MakeRectangleVao()
{
  glGenVertexArrays(1, &rectangleVao_);
  glBindVertexArray(rectangleVao_);

  glGenBuffers(1, &rectangleVertexBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER, rectangleVertexBuffer_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices_), rectangleVertices_,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
}

void
GlRenderer::ResetMatrix()
{
  worldMatrix_ = glm::mat4(1.0f);
}

void
GlRenderer::InitLighting()
{
  lightDirection_ = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
  lightColor_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
  ambientLightColor_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
}

void
GlRenderer::InitMaterial()
{
  Color(0, 0, 0.8f);
}

void
GlRenderer::UpdateUniformsForObject()
{
  auto worldViewProjection = camera_.ViewProjectionMatrix() * worldMatrix_;
  auto inverseWorldMatrix = glm::inverse(worldMatrix_);
  auto modelSpaceLightDirection =
    glm::normalize(inverseWorldMatrix * lightDirection_);

  glUniformMatrix4fv(worldViewProjectionLoc_, 1, GL_FALSE,
                     glm::value_ptr(worldViewProjection));
  glUniform4fv(lightDirectionLoc_, 1, glm::value_ptr(modelSpaceLightDirection));
  glUniform4fv(lightColorLoc_, 1, glm::value_ptr(lightColor_));
  glUniform4fv(ambientLightColorLoc_, 1, glm::value_ptr(ambientLightColor_));
  glUniform4f(materialColorLoc_, materialR_, materialG_, materialB_,
              materialA_);
}
}
