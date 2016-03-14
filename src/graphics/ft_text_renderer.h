#pragma once

#include "font_utils.h"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <array>
#include <string>
#include <vector>

#define FT_TEXT_RENDERER_MAX_DRAW_GLYPHS 1000
#define FT_TEXT_RENDERER_VERTICES_PER_GLYPH 6
#define FT_TEXT_RENDERER_COMPONENTS_PER_VERTEX 4

#define FT_TEXT_RENDERER_MAX_ASCII_CODE 126

namespace procdraw {

    class FtTextRenderer {
    public:
        FtTextRenderer();
        ~FtTextRenderer();
        void BeginText(int width, int height);
        void CalculateBlockCursorPos(int cursorTextPosition, int *x, int *width, int *height);
        void DrawText(int x, int y, const std::vector<GLfloat> &vertices);
        int GetLinespace();
        void LayOutText(const std::string &text, std::vector<GLfloat> &vertices);
    private:
        FT_Library ft_;
        FT_Face face_;
        GLuint program_;
        GLint projectionLoc_;
        GLint texLoc_;
        GLuint glyphQuadVertexBuffer_;
        GLuint glyphQuadVao_;
        glm::mat4 orthoProjection_;
        TextureFontMetrics asciiFontMetrics_;
        GLuint asciiFontTexture_;
        void Cleanup();
        void CompileShaders();
        void MakeGlyphQuadVao();
        void MakeFontTexture(FT_ULong fromCharCode, FT_ULong toCharCode,
                             TextureFontMetrics &fontMetrics, GLuint *fontTexture);
        void CalculateTextureSize(FT_ULong fromCharCode, FT_ULong toCharCode,
                                  int *width, int *height);
        void PopulateTexture(FT_ULong fromCharCode, FT_ULong toCharCode, TextureFontMetrics &fontMetrics);
        void RenderChar(FT_ULong charCode);
    };

}
