#pragma once

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace procdraw {

    class FtTextRenderer {
    public:
        FtTextRenderer();
        ~FtTextRenderer();
        void BeginText(int width, int height);
        void Text(int x, int y);
    private:
        FT_Library ft_;
        FT_Face face_;
        GLuint program_;
        GLuint textRectangleVertexBuffer_;
        GLuint textRectangleVao_;
        GLfloat textRectangleVertices_[16] = {};
        GLuint textTexture_;
        void Cleanup();
        void CompileShaders();
        void MakeTextRectangleVao();
        void MakeTextTexture();
        FT_GlyphSlot LoadChar();
    };

}
