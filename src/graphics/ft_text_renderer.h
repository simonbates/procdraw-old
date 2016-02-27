#pragma once

#include "font_utils.h"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <array>
#include <string>

#define FT_TEXT_RENDERER_MAX_DRAW_GLYPHS 1000
#define FT_TEXT_RENDERER_VERTICES_PER_QUAD 24
#define FT_TEXT_RENDERER_MAX_ASCII_CODE 126

// TODO: Do some refactoring and pull bits out, such as font metrics type
//       So that I can support multiple fonts and switch between

namespace procdraw {

    class FtTextRenderer {
    public:
        FtTextRenderer();
        ~FtTextRenderer();
        void BeginText(int width, int height);
        void CalculateBlockCursorPos(int cursorTextPosition, int *x, int *width, int *height);
        void Text(int x, int y, const std::string &text);
    private:
        FT_Library ft_;
        FT_Face face_;
        GLuint program_;
        GLint projectionLoc_;
        GLint texLoc_;
        GLuint glyphQuadVertexBuffer_;
        GLuint glyphQuadVao_;
        GLfloat glyphQuadVertices_[FT_TEXT_RENDERER_VERTICES_PER_QUAD * FT_TEXT_RENDERER_MAX_DRAW_GLYPHS] = {};
        int asciiFontAscenderPixels_ = 0;
        int asciiFontDescenderPixels_ = 0;
        GLuint asciiFontTexture_;
        GLsizei asciiFontTextureWidth_ = 0;
        GLsizei asciiFontTextureHeight_ = 0;
        std::array<TextureGlyphMetrics, FT_TEXT_RENDERER_MAX_ASCII_CODE + 1> asciiGlyphMetrics;
        void Cleanup();
        void CompileShaders();
        void MakeGlyphQuadVao();
        void MakeAsciiFontTexture();
        void CalculateTextureSize(FT_ULong fromCharCode, FT_ULong toCharCode,
                                  int *width, int *height);
        template<size_t N>
        void PopulateTexture(FT_ULong fromCharCode, FT_ULong toCharCode, std::array<TextureGlyphMetrics, N> &metrics);
        void RenderChar(FT_ULong charCode);
    };

    template<size_t N>
    void FtTextRenderer::PopulateTexture(FT_ULong fromCharCode, FT_ULong toCharCode,
                                         std::array<TextureGlyphMetrics, N> &metrics)
    {
        GLint xoffset = 0;

        for (auto charCode = fromCharCode; charCode < toCharCode; ++charCode) {
            RenderChar(charCode);
            FT_GlyphSlot g = face_->glyph;

            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            xoffset,
                            0,
                            g->bitmap.width,
                            g->bitmap.rows,
                            GL_RED,
                            GL_UNSIGNED_BYTE,
                            g->bitmap.buffer);

            metrics.at(charCode).xoffsetPixels = xoffset;
            metrics.at(charCode).widthPixels = g->bitmap.width;
            metrics.at(charCode).heightPixels = g->bitmap.rows;
            metrics.at(charCode).advanceWidthPixels = g->advance.x / 64;
            metrics.at(charCode).leftBearingPixels = g->bitmap_left;
            metrics.at(charCode).topBearingPixels = g->bitmap_top;

            xoffset += g->bitmap.width;
        }
    }

}
