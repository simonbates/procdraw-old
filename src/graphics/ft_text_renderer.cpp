#define GLM_FORCE_RADIANS

#include "ft_text_renderer.h"
#include "font_config.h"
#include "gl_util.h"
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
            MakeFontTexture();
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
        glDeleteTextures(1, &fontTexture_);
        glDeleteProgram(program_);
    }

    void FtTextRenderer::BeginText(int width, int height)
    {
        glUseProgram(program_);
        // TODO: Cache the 2d projection matrix -- no need to calculate
        // each time, only when the renderer size changes
        auto projection = glm::ortho(0.0f, static_cast<float>(width),
                                     static_cast<float>(height), 0.0f);
        glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(texLoc_, 0);
        glDisable(GL_DEPTH_TEST);
    }

    void FtTextRenderer::Text(int x, int y)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fontTexture_);

        // Top left
        glyphQuadVertices_[0] = x;
        glyphQuadVertices_[1] = y;
        glyphQuadVertices_[2] = 0.0f;
        glyphQuadVertices_[3] = 0.0f;
        // Bottom left
        glyphQuadVertices_[4] = x;
        glyphQuadVertices_[5] = y + fontTextureHeight_;
        glyphQuadVertices_[6] = 0.0f;
        glyphQuadVertices_[7] = 1.0f;
        // Top right
        glyphQuadVertices_[8] = x + fontTextureWidth_;
        glyphQuadVertices_[9] = y;
        glyphQuadVertices_[10] = 1.0f;
        glyphQuadVertices_[11] = 0.0f;
        // Bottom right
        glyphQuadVertices_[12] = x + fontTextureWidth_;
        glyphQuadVertices_[13] = y + fontTextureHeight_;
        glyphQuadVertices_[14] = 1.0f;
        glyphQuadVertices_[15] = 1.0f;

        glBindVertexArray(glyphQuadVao_);
        glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glyphQuadVertices_), glyphQuadVertices_);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
            "    color = vec4(0, 0, 0, texture(tex, tc).r);     \n"
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

        glGenBuffers(1, &glyphQuadVertexBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glyphQuadVertices_), glyphQuadVertices_, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

    void FtTextRenderer::MakeFontTexture()
    {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &fontTexture_);
        glBindTexture(GL_TEXTURE_2D, fontTexture_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_ULong fromCharCode = 'a';
        FT_ULong toCharCode = 'f';

        CalculateTextureSize(fromCharCode, toCharCode, &fontTextureWidth_, &fontTextureHeight_);
        // TODO: Make the texture dimensions powers of 2

        // Allocate the texture memory
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     fontTextureWidth_,
                     fontTextureHeight_,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     nullptr);

        PopulateTexture(fromCharCode, toCharCode);
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
    }

    // TODO: Rename PopulateTexture to PopulateTextureAndMetrics
    // TODO: Pass in the address of the metrics array to populate
    void FtTextRenderer::PopulateTexture(FT_ULong fromCharCode, FT_ULong toCharCode)
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

            // TODO: Populate the glyphMetrics array

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
