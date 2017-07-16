#pragma once

#include "procdraw/graphics/freetype_font_loader.h"
#include "procdraw/text_layout_engine.h"
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <string>

namespace procdraw {

const int BitmapTextRendererMaxDrawGlyphs = 1000;
const int BitmapTextRendererVerticesPerGlyph = 6;
const int BitmapTextRendererComponentsPerVertex = 4;

class BitmapTextRenderer {
public:
    BitmapTextRenderer();
    ~BitmapTextRenderer();
    void BeginText(int width, int height);
    void DrawText(int x,
                  int y,
                  const TextLayout<GLfloat>& layout,
                  TextLayout<GLfloat>::size_type startLineNum,
                  TextLayout<GLfloat>::size_type endLineNum);
    int GetLinespace();
    TextLayout<GLfloat> LayOutText(const std::string& text,
                                   int maxLineWidthPixels);

private:
    GLuint program_;
    GLint projectionLoc_;
    GLint texLoc_;
    GLuint glyphQuadVertexBuffer_;
    GLuint glyphQuadVao_;
    glm::mat4 orthoProjection_;
    TextLayoutEngine<GLfloat> textLayoutEngine_;
    FreeTypeFontLoader fontLoader_;
    BitmapFontMetrics monospaceFontMetrics_;
    GLuint monospaceFontTexture_;
    void Cleanup();
    void CompileShaders();
    void MakeGlyphQuadVao();
};
}
