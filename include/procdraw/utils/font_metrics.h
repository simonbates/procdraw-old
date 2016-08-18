#pragma once

#include <vector>

namespace procdraw {

    struct TextureGlyphMetrics {
        // Location of the glyph in the texture
        int XoffsetPixels;
        // Width of the glyph bitmap
        int WidthPixels;
        // Height of the glyph bitmap
        int HeightPixels;
        // Distance to advance the drawing position for next glyph
        int AdvanceWidthPixels;
        // Distance from the drawing position to the left of the glyph's bitmap
        int LeftBearingPixels;
        // Distance from the baseline to the top of the glyph's bitmap
        int TopBearingPixels;
        TextureGlyphMetrics()
        {
            this->XoffsetPixels = 0;
            this->WidthPixels = 0;
            this->HeightPixels = 0;
            this->AdvanceWidthPixels = 0;
            this->LeftBearingPixels = 0;
            this->TopBearingPixels = 0;
        }
    };

    class TextureFontMetrics {
    public:
        // Distance from baseline to highest point in face
        int AscenderPixels = 0;
        // Distance from baseline to lowest point in face (negative)
        int DescenderPixels = 0;
        // Vertical distance between two consecutive baselines
        int LinespacePixels = 0;
        int TextureWidth = 0;
        int TextureHeight = 0;
        void ClearGlyphs(int maxCharCode);
        int MaxCharCode();
        void SetGlyph(int charCode, const TextureGlyphMetrics &glyphMetrics);
        TextureGlyphMetrics& GetGlyph(int charCode);
    private:
        std::vector<TextureGlyphMetrics> glyphMetrics_;
    };

}
