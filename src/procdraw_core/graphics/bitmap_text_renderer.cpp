#define GLM_FORCE_RADIANS

#include "procdraw_core/graphics/bitmap_text_renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "procdraw_core/graphics/font_config.h"
#include "procdraw_core/graphics/gl_util.h"
#include <string>

namespace procdraw {

BitmapTextRenderer::BitmapTextRenderer()
{
  try {
    CompileShaders();
    MakeGlyphQuadVao();
    fontLoader_.LoadFont(MonospaceFontFilePath, MonospaceFontSizePixels,
                         &monospaceFontMetrics_, &monospaceFontTexture_);
  } catch (...) {
    Cleanup();
    throw;
  }
}

BitmapTextRenderer::~BitmapTextRenderer()
{
  Cleanup();
}

void
BitmapTextRenderer::Cleanup()
{
  glDeleteVertexArrays(1, &glyphQuadVao_);
  glDeleteBuffers(1, &glyphQuadVertexBuffer_);
  glDeleteTextures(1, &monospaceFontTexture_);
  glDeleteProgram(program_);
}

void
BitmapTextRenderer::BeginText(int width, int height)
{
  glUseProgram(program_);
  // TODO: Cache the 2d projection matrix -- no need to calculate
  // each time, only when the renderer size changes
  orthoProjection_ = glm::ortho(0.0f, static_cast<float>(width),
                                static_cast<float>(height), 0.0f);
  glUniform1i(texLoc_, 0);
  glDisable(GL_DEPTH_TEST);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, monospaceFontTexture_);
  glBindVertexArray(glyphQuadVao_);
  glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
}

void
BitmapTextRenderer::DrawText(int x, int y, const TextLayout<GLfloat>& layout,
                             TextLayout<GLfloat>::size_type startLineNum,
                             TextLayout<GLfloat>::size_type endLineNum)
{
  int startLineY = y - (startLineNum * layout.LinespacePixels);
  auto projectionMatrix =
    glm::translate(orthoProjection_, glm::vec3(x, startLineY, 0));
  glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE,
                     glm::value_ptr(projectionMatrix));

  for (std::remove_reference<decltype(layout)>::type::size_type i =
         startLineNum;
       i < endLineNum; ++i) {
    auto vertices = layout.GetVerticesForLine(i);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertices.size(),
                    vertices.data());
    glDrawArrays(GL_TRIANGLES, 0,
                 vertices.size() / BitmapTextRendererComponentsPerVertex);
  }
}

int
BitmapTextRenderer::GetLinespace()
{
  return monospaceFontMetrics_.LinespacePixels;
}

TextLayout<GLfloat>
BitmapTextRenderer::LayOutText(const std::string& text, int maxLineWidthPixels)
{
  return textLayoutEngine_.LayOutText(text, monospaceFontMetrics_,
                                      BitmapTextRendererMaxDrawGlyphs,
                                      maxLineWidthPixels);
}

void
BitmapTextRenderer::CompileShaders()
{
  static const GLchar* vertexShaderSource[] = {
    "#version 150                                                       \n"
    "uniform mat4 projection;                                           \n"
    "in vec4 position;                                                  \n"
    "out vec2 tc;                                                       \n"
    "void main(void)                                                    \n"
    "{                                                                  \n"
    "  gl_Position = projection * vec4(position.xy, 0, 1);              \n"
    "  tc = position.zw;                                                \n"
    "}                                                                  \n"
  };

  static const GLchar* fragmentShaderSource[] = {
    "#version 150                                                       \n"
    "uniform sampler2D tex;                                             \n"
    "in vec2 tc;                                                        \n"
    "out vec4 color;                                                    \n"
    "void main(void)                                                    \n"
    "{                                                                  \n"
    "  color = vec4(0.96875, 0.96875, 0.96875, texture(tex, tc).r);     \n"
    "}                                                                  \n"
  };

  program_ = CompileProgram(vertexShaderSource, fragmentShaderSource,
                            { { 0, "position" } });
  projectionLoc_ = glGetUniformLocation(program_, "projection");
  texLoc_ = glGetUniformLocation(program_, "tex");
}

void
BitmapTextRenderer::MakeGlyphQuadVao()
{
  glGenVertexArrays(1, &glyphQuadVao_);
  glBindVertexArray(glyphQuadVao_);

  auto vertexBufferSize = sizeof(GLfloat) * BitmapTextRendererMaxDrawGlyphs *
                          BitmapTextRendererVerticesPerGlyph *
                          BitmapTextRendererComponentsPerVertex;

  glGenBuffers(1, &glyphQuadVertexBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
  glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, BitmapTextRendererComponentsPerVertex, GL_FLOAT,
                        GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
}
}
