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
            MakeAsciiFontTexture();
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
        auto projection = glm::ortho(0.0f, static_cast<float>(width),
                                     static_cast<float>(height), 0.0f);
        glUniformMatrix4fv(projectionLoc_, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(texLoc_, 0);
        glDisable(GL_DEPTH_TEST);
    }

    void FtTextRenderer::CalculateBlockCursorPos(int cursorTextPosition, int *x, int *width)
    {
        CalculateFixedWidthBlockCursorPos(cursorTextPosition, asciiGlyphMetrics[32].advanceWidthPixels, x, width);
    }

    // TODO: Split the Text method into 2 -- (1) layout and (2) draw layout
    // Then I can cache layouts for text -- very little text will change every frame

    void FtTextRenderer::Text(int x, int y, const std::string &text)
    {
        // TODO: Calculate the baseline position from font metrics
        float baseline = 80;

        int cursorX = x;
        int numGlyphs = 0;
        int verticesOffset = 0;
        int maxCharCode = asciiGlyphMetrics.size() - 1;

        for (const char &ch : text) {
            if (numGlyphs >= FT_TEXT_RENDERER_MAX_DRAW_GLYPHS) {
                break;
            }

            if (ch <= 32 || ch > maxCharCode) {
                cursorX += asciiGlyphMetrics[32].advanceWidthPixels;
                continue;
            }

            unsigned int charCode = ch;

            float glyphWidth = asciiGlyphMetrics[charCode].widthPixels;
            float glyphHeight = asciiGlyphMetrics[charCode].heightPixels;

            float glyphLeft = cursorX + asciiGlyphMetrics[charCode].leftBearingPixels;
            float glyphRight = glyphLeft + glyphWidth;
            float glyphTop = y + baseline - asciiGlyphMetrics[charCode].topBearingPixels;
            float glyphBottom = glyphTop + glyphHeight;

            float glyphTextureLeft = ((float)asciiGlyphMetrics[charCode].xoffsetPixels) / asciiFontTextureWidth_;
            float glyphTextureRight = (asciiGlyphMetrics[charCode].xoffsetPixels + glyphWidth) / asciiFontTextureWidth_;
            float glyphTextureTop = 0.0f;
            float glyphTextureBottom = glyphHeight / asciiFontTextureHeight_;

            // Top left
            glyphQuadVertices_[verticesOffset + 0] = glyphLeft;
            glyphQuadVertices_[verticesOffset + 1] = glyphTop;
            glyphQuadVertices_[verticesOffset + 2] = glyphTextureLeft;
            glyphQuadVertices_[verticesOffset + 3] = glyphTextureTop;
            // Bottom left
            glyphQuadVertices_[verticesOffset + 4] = glyphLeft;
            glyphQuadVertices_[verticesOffset + 5] = glyphBottom;
            glyphQuadVertices_[verticesOffset + 6] = glyphTextureLeft;
            glyphQuadVertices_[verticesOffset + 7] = glyphTextureBottom;
            // Top right
            glyphQuadVertices_[verticesOffset + 8] = glyphRight;
            glyphQuadVertices_[verticesOffset + 9] = glyphTop;
            glyphQuadVertices_[verticesOffset + 10] = glyphTextureRight;
            glyphQuadVertices_[verticesOffset + 11] = glyphTextureTop;

            // Bottom left
            glyphQuadVertices_[verticesOffset + 12] = glyphLeft;
            glyphQuadVertices_[verticesOffset + 13] = glyphBottom;
            glyphQuadVertices_[verticesOffset + 14] = glyphTextureLeft;
            glyphQuadVertices_[verticesOffset + 15] = glyphTextureBottom;
            // Bottom right
            glyphQuadVertices_[verticesOffset + 16] = glyphRight;
            glyphQuadVertices_[verticesOffset + 17] = glyphBottom;
            glyphQuadVertices_[verticesOffset + 18] = glyphTextureRight;
            glyphQuadVertices_[verticesOffset + 19] = glyphTextureBottom;
            // Top right
            glyphQuadVertices_[verticesOffset + 20] = glyphRight;
            glyphQuadVertices_[verticesOffset + 21] = glyphTop;
            glyphQuadVertices_[verticesOffset + 22] = glyphTextureRight;
            glyphQuadVertices_[verticesOffset + 23] = glyphTextureTop;

            cursorX += asciiGlyphMetrics[charCode].advanceWidthPixels;
            ++numGlyphs;
            verticesOffset += FT_TEXT_RENDERER_VERTICES_PER_QUAD;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asciiFontTexture_);
        glBindVertexArray(glyphQuadVao_);
        glBindBuffer(GL_ARRAY_BUFFER, glyphQuadVertexBuffer_);
        // TODO: Copy only as much of glyphQuadVertices_ as I need
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glyphQuadVertices_), glyphQuadVertices_);
        glDrawArrays(GL_TRIANGLES, 0, 6 * numGlyphs);
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

    void FtTextRenderer::MakeAsciiFontTexture()
    {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &asciiFontTexture_);
        glBindTexture(GL_TEXTURE_2D, asciiFontTexture_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_ULong fromCharCode = 32;
        FT_ULong toCharCode = asciiGlyphMetrics.size();

        CalculateTextureSize(fromCharCode, toCharCode, &asciiFontTextureWidth_, &asciiFontTextureHeight_);
        // TODO: Make the texture dimensions powers of 2

        // Allocate the texture memory
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     asciiFontTextureWidth_,
                     asciiFontTextureHeight_,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     nullptr);

        PopulateTexture(fromCharCode, toCharCode, asciiGlyphMetrics);
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

    void FtTextRenderer::RenderChar(FT_ULong charCode)
    {
        if (FT_Load_Char(face_, charCode, FT_LOAD_RENDER)) {
            throw std::runtime_error("Error FT_Load_Char");
        }
    }

}
