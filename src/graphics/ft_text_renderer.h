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

// TODO: Extract font metrics to a new type TextureFontMetrics

namespace procdraw {

    class FtTextRenderer {
    public:
        FtTextRenderer();
        ~FtTextRenderer();
        void BeginText(int width, int height);
        void CalculateBlockCursorPos(int cursorTextPosition, int *x, int *width, int *height);
        void DrawText(int x, int y, const std::vector<GLfloat> &vertices);
        int GetLinespace();
        void LayoutText(const std::string &text, std::vector<GLfloat> &vertices);
    private:
        FT_Library ft_;
        FT_Face face_;
        GLuint program_;
        GLint projectionLoc_;
        GLint texLoc_;
        GLuint glyphQuadVertexBuffer_;
        GLuint glyphQuadVao_;
        glm::mat4 orthoProjection_;
        int asciiFontAscenderPixels_ = 0;
        int asciiFontDescenderPixels_ = 0;
        int asciiFontLinespacePixels_ = 0;
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
