#define GLM_FORCE_RADIANS

#include "ft_text_renderer.h"
#include "font_config.h"
#include "gl_util.h"
#include "utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace procdraw {

    FtTextRenderer::FtTextRenderer()
    {
        try {
            if (FT_Init_FreeType(&ft_)) {
                throw std::runtime_error("Error FT_Init_FreeType");
            }

            if (FT_New_Face(ft_, FT_TEXT_RENDERER_FONT_PATHNAME, 0, &face_)) {
                throw std::runtime_error("Error loading font " FT_TEXT_RENDERER_FONT_PATHNAME);
            }

            FT_Set_Pixel_Sizes(face_, 0, FT_TEXT_RENDERER_FONT_SIZE_PIXELS);

            CompileShaders();
            MakeGlyphQuadVao();

            // Build font data
            asciiFontMetrics_.ascenderPixels = face_->size->metrics.ascender / 64;
            asciiFontMetrics_.descenderPixels = face_->size->metrics.descender / 64;
            asciiFontMetrics_.linespacePixels = face_->size->metrics.height / 64;
            MakeFontTexture(32, FT_TEXT_RENDERER_MAX_ASCII_CODE, asciiFontMetrics_, &asciiFontTexture_);
        }
        catch (...) {
            Cleanup();
            throw;
        }
    }

    FtTextRenderer::~FtTextRenderer()
    {
        Cleanup();
    }

    void FtTextRenderer::Cleanup()
    {
        glDeleteVertexArrays(1, &glyphQuadVao_);
        glDeleteBuffers(1, &glyphQuadVertexBuffer_);
        glDeleteTextures(1, &asciiFontTexture_);
        glDeleteProgram(program_);
    }

    void FtTextRenderer::BeginText(int width, int height)
    {
        glUseProgram(program_);
        // TODO: Cache the 2d projection matrix -- no need to calculate
        // each time, only when the renderer size changes
        orthoProjection_ = glm::ortho(0.0f, static_cast<float>(width),
                                      static_cast<float>(height), 0.0f);
        glUniform1i(texLoc_, 0);
        glDisable(GL_DEPTH_TEST);
    }

    void FtTextRenderer::CalculateBlockCursorPos(int cursorTextPosition, int *x, int *width, int *height)
    {
        CalculateFixedWidthBlockCursorPos(cursorTextPosition, asciiFontMetrics_.GetGlyph(32).advanceWidthPixels, x, width);
        *height = asciiFontMetrics_.ascenderPixels - asciiFontMetrics_.descenderPixels;
    }

    void FtTextRenderer::DrawText(int x, int y, const std::vector<GLfloat> &vertices)
    {
        auto projectionMatrix = glm::translate(orthoProjection_, glm::vec3(x, y, 0));
        glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asciiFontTexture_);
        glBindVertexArray(glyphQuadVao_);
        glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertices.size(), vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / FT_TEXT_RENDERER_COMPONENTS_PER_VERTEX);
    }

    int FtTextRenderer::GetLinespace()
    {
        return asciiFontMetrics_.linespacePixels;
    }

    // TODO: Move LayoutText to utils/font_utils -- add new TextureFontMetrics parameter and write unit tests

    void FtTextRenderer::LayoutText(const std::string &text, std::vector<GLfloat> &vertices)
    {
        vertices.clear();

        int cursorX = 0;
        int numGlyphs = 0;
        int maxCharCode = asciiFontMetrics_.MaxCharCode();

        for (const char &ch : text) {
            if (numGlyphs >= FT_TEXT_RENDERER_MAX_DRAW_GLYPHS) {
                break;
            }

            if (ch <= 32 || ch > maxCharCode) {
                cursorX += asciiFontMetrics_.GetGlyph(32).advanceWidthPixels;
                continue;
            }

            unsigned int charCode = ch;

            float glyphWidth = asciiFontMetrics_.GetGlyph(charCode).widthPixels;
            float glyphHeight = asciiFontMetrics_.GetGlyph(charCode).heightPixels;

            float glyphLeft = cursorX + asciiFontMetrics_.GetGlyph(charCode).leftBearingPixels;
            float glyphRight = glyphLeft + glyphWidth;
            float glyphTop = asciiFontMetrics_.ascenderPixels - asciiFontMetrics_.GetGlyph(charCode).topBearingPixels;
            float glyphBottom = glyphTop + glyphHeight;

            float glyphTextureLeft = ((float)asciiFontMetrics_.GetGlyph(charCode).xoffsetPixels) / asciiFontMetrics_.textureWidth;
            float glyphTextureRight = (asciiFontMetrics_.GetGlyph(charCode).xoffsetPixels + glyphWidth) /
                                      asciiFontMetrics_.textureWidth;
            float glyphTextureTop = 0.0f;
            float glyphTextureBottom = glyphHeight / asciiFontMetrics_.textureHeight;

            // Top left
            vertices.push_back(glyphLeft);
            vertices.push_back(glyphTop);
            vertices.push_back(glyphTextureLeft);
            vertices.push_back(glyphTextureTop);
            // Bottom left
            vertices.push_back(glyphLeft);
            vertices.push_back(glyphBottom);
            vertices.push_back(glyphTextureLeft);
            vertices.push_back(glyphTextureBottom);
            // Top right
            vertices.push_back(glyphRight);
            vertices.push_back(glyphTop);
            vertices.push_back(glyphTextureRight);
            vertices.push_back(glyphTextureTop);

            // Bottom left
            vertices.push_back(glyphLeft);
            vertices.push_back(glyphBottom);
            vertices.push_back(glyphTextureLeft);
            vertices.push_back(glyphTextureBottom);
            // Bottom right
            vertices.push_back(glyphRight);
            vertices.push_back(glyphBottom);
            vertices.push_back(glyphTextureRight);
            vertices.push_back(glyphTextureBottom);
            // Top right
            vertices.push_back(glyphRight);
            vertices.push_back(glyphTop);
            vertices.push_back(glyphTextureRight);
            vertices.push_back(glyphTextureTop);

            cursorX += asciiFontMetrics_.GetGlyph(charCode).advanceWidthPixels;
            ++numGlyphs;
        }
    }

    void FtTextRenderer::CompileShaders()
    {
        static const GLchar *vertexShaderSource[] = {
            "#version 150                                                               \n"
            "uniform mat4 projection;                                                   \n"
            "in vec4 position;                                                          \n"
            "out vec2 tc;                                                               \n"
            "void main(void)                                                            \n"
            "{                                                                          \n"
            "    gl_Position = projection * vec4(position.xy, 0, 1);                    \n"
            "    tc = position.zw;                                                      \n"
            "}                                                                          \n"
        };

        static const GLchar *fragmentShaderSource[] = {
            "#version 150                                       \n"
            "uniform sampler2D tex;                             \n"
            "in vec2 tc;                                        \n"
            "out vec4 color;                                    \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "    color = vec4(0.96875, 0.96875, 0.96875, texture(tex, tc).r);           \n"
            "}                                                  \n"
        };

        program_ = CompileProgram(vertexShaderSource, fragmentShaderSource, {{0, "position"}});
        projectionLoc_ = glGetUniformLocation(program_, "projection");
        texLoc_ = glGetUniformLocation(program_, "tex");
    }

    void FtTextRenderer::MakeGlyphQuadVao()
    {
        glGenVertexArrays(1, &glyphQuadVao_);
        glBindVertexArray(glyphQuadVao_);

        auto vertexBufferSize = sizeof(GLfloat) *
                                FT_TEXT_RENDERER_MAX_DRAW_GLYPHS *
                                FT_TEXT_RENDERER_VERTICES_PER_GLYPH *
                                FT_TEXT_RENDERER_COMPONENTS_PER_VERTEX;

        glGenBuffers(1, &glyphQuadVertexBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, FT_TEXT_RENDERER_COMPONENTS_PER_VERTEX, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

    void FtTextRenderer::MakeFontTexture(FT_ULong fromCharCode,
                                         FT_ULong toCharCode,
                                         TextureFontMetrics &fontMetrics,
                                         GLuint *fontTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, fontTexture);
        glBindTexture(GL_TEXTURE_2D, *fontTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        CalculateTextureSize(fromCharCode, toCharCode, &(fontMetrics.textureWidth), &(fontMetrics.textureHeight));

        // Allocate the texture memory
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     fontMetrics.textureWidth,
                     fontMetrics.textureHeight,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     nullptr);

        PopulateTexture(fromCharCode, toCharCode, fontMetrics);
    }

    void FtTextRenderer::CalculateTextureSize(FT_ULong fromCharCode, FT_ULong toCharCode,
            GLsizei *width, GLsizei *height)
    {
        *width = 0;
        *height = 0;
        for (auto charCode = fromCharCode; charCode < toCharCode; ++charCode) {
            RenderChar(charCode);
            FT_GlyphSlot g = face_->glyph;
            *width += g->bitmap.width;
            if (g->bitmap.rows > *height) {
                *height = g->bitmap.rows;
            }
        }
        // Ensure that the texture dimensions are powers of 2
        *width = PowerOf2Gte(*width);
        *height = PowerOf2Gte(*height);
    }

    void FtTextRenderer::PopulateTexture(FT_ULong fromCharCode, FT_ULong toCharCode,
                                         TextureFontMetrics &fontMetrics)
    {
        fontMetrics.ClearGlyphs(toCharCode);

        GLint xoffset = 0;

        TextureGlyphMetrics glyphMetrics;

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

            glyphMetrics.xoffsetPixels = xoffset;
            glyphMetrics.widthPixels = g->bitmap.width;
            glyphMetrics.heightPixels = g->bitmap.rows;
            glyphMetrics.advanceWidthPixels = g->advance.x / 64;
            glyphMetrics.leftBearingPixels = g->bitmap_left;
            glyphMetrics.topBearingPixels = g->bitmap_top;

            fontMetrics.SetGlyph(charCode, glyphMetrics);

            xoffset += g->bitmap.width;
        }
    }

    void FtTextRenderer::RenderChar(FT_ULong charCode)
    {
        if (FT_Load_Char(face_, charCode, FT_LOAD_RENDER)) {
            throw std::runtime_error("Error FT_Load_Char");
        }
    }

}
