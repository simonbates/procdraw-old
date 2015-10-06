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
        GLint projectionLoc_;
        GLint texLoc_;
        GLuint glyphQuadVertexBuffer_;
        GLuint glyphQuadVao_;
        GLfloat glyphQuadVertices_[16] = {};
        GLuint fontTexture_;
        GLsizei fontTextureWidth_ = 0;
        GLsizei fontTextureHeight_ = 0;
        void Cleanup();
        void CompileShaders();
        void MakeGlyphQuadVao();
        void MakeFontTexture();
        void CalculateTextureSize(FT_ULong fromCharCode, FT_ULong toCharCode,
                                  int *width, int *height);
        void PopulateTexture(FT_ULong fromCharCode, FT_ULong toCharCode);
        void RenderChar(FT_ULong charCode);
    };

}
