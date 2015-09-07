#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace procdraw {

    class FtTextRenderer {
    public:
        FtTextRenderer();
        FT_GlyphSlot LoadChar();
    private:
        FT_Library ft_;
        FT_Face face_;
    };

}
