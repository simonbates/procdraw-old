#include "ft_text_renderer.h"
#include <stdexcept>

namespace procdraw {

    FtTextRenderer::FtTextRenderer()
    {
        if (FT_Init_FreeType(&ft_)) {
            throw std::runtime_error("Error FT_Init_FreeType");
        }

        // TODO Need another solution, rather than hardcoding the font path
        if (FT_New_Face(ft_, "/usr/share/fonts/truetype/DejaVuSansMono.ttf", 0, &face_)) {
            throw std::runtime_error("Error FT_New_Face");
        }

        FT_Set_Pixel_Sizes(face_, 0, 240);
    }

    FT_GlyphSlot FtTextRenderer::LoadChar()
    {
        if (FT_Load_Char(face_, 'Q', FT_LOAD_RENDER)) {
            throw std::runtime_error("Error FT_Load_Char");
        }
        return face_->glyph;
    }

}
