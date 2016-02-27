#pragma once

namespace procdraw {

    struct TextureGlyphMetrics {
        // Location of the glyph in the texture
        int xoffsetPixels;
        // Width of the glyph bitmap
        int widthPixels;
        // Height of the glyph bitmap
        int heightPixels;
        // Distance to advance the drawing position for next glyph
        int advanceWidthPixels;
        // Distance from the drawing position to the left of the glyph's bitmap
        int leftBearingPixels;
        // Distance from the baseline to the top of the glyph's bitmap
        int topBearingPixels;
        TextureGlyphMetrics()
        {
            this->xoffsetPixels = 0;
            this->widthPixels = 0;
            this->heightPixels = 0;
            this->advanceWidthPixels = 0;
            this->leftBearingPixels = 0;
            this->topBearingPixels = 0;
        }
    };

    void CalculateFixedWidthBlockCursorPos(int cursorTextPosition, int glyphWidth, int *x, int *width);

}
