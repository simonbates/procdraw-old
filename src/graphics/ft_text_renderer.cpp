#define GLM_FORCE_RADIANS

#include "procdraw/graphics/ft_text_renderer.h"
#include "procdraw/graphics/font_config.h"
#include "procdraw/graphics/gl_util.h"
#include "procdraw/utils/utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdexcept>
#include <type_traits>

namespace procdraw {

    FtTextRenderer::FtTextRenderer()
    {
        try {
            if (FT_Init_FreeType(&ft_)) {
                throw std::runtime_error("Error FT_Init_FreeType");
            }

            if (FT_New_Face(ft_, CodeFontFilePath.c_str(), 0, &face_)) {
                throw std::runtime_error("Error loading font " + CodeFontFilePath);
            }

            FT_Set_Pixel_Sizes(face_, 0, CodeFontSizePixels);

            CompileShaders();
            MakeGlyphQuadVao();

            // Build font data
            asciiFontMetrics_.AscenderPixels = face_->size->metrics.ascender / 64;
            asciiFontMetrics_.DescenderPixels = face_->size->metrics.descender / 64;
            asciiFontMetrics_.LinespacePixels = face_->size->metrics.height / 64;
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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asciiFontTexture_);
        glBindVertexArray(glyphQuadVao_);
        glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
    }

    void FtTextRenderer::DrawText(int x, int y, const TextLayout<GLfloat> &layout,
                                  TextLayout<GLfloat>::size_type startLineNum,
                                  TextLayout<GLfloat>::size_type endLineNum)
    {
        int startLineY = y - (startLineNum * layout.LinespacePixels);
        auto projectionMatrix = glm::translate(orthoProjection_, glm::vec3(x, startLineY, 0));
        glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        for (std::remove_reference<decltype(layout)>::type::size_type i = startLineNum; i < endLineNum; ++i) {
            auto vertices = layout.GetVerticesForLine(i);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertices.size(), vertices.data());
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / FT_TEXT_RENDERER_COMPONENTS_PER_VERTEX);
        }
    }

    int FtTextRenderer::GetLinespace()
    {
        return asciiFontMetrics_.LinespacePixels;
    }

    TextLayout<GLfloat> FtTextRenderer::LayOutText(const std::string &text, int maxLineWidthPixels)
    {
        return procdraw::LayOutText<GLfloat>(text, asciiFontMetrics_,
                                             FT_TEXT_RENDERER_MAX_DRAW_GLYPHS,
                                             maxLineWidthPixels);
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

        CalculateTextureSize(fromCharCode, toCharCode, &(fontMetrics.TextureWidth), &(fontMetrics.TextureHeight));

        // Allocate the texture memory
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     fontMetrics.TextureWidth,
                     fontMetrics.TextureHeight,
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

            glyphMetrics.XoffsetPixels = xoffset;
            glyphMetrics.WidthPixels = g->bitmap.width;
            glyphMetrics.HeightPixels = g->bitmap.rows;
            glyphMetrics.AdvanceWidthPixels = g->advance.x / 64;
            glyphMetrics.LeftBearingPixels = g->bitmap_left;
            glyphMetrics.TopBearingPixels = g->bitmap_top;

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
