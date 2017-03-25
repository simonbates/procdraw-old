#pragma once

namespace procdraw {

struct BitmapGlyphMetrics {
    // Location of the glyph in the bitmap
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
    BitmapGlyphMetrics()
    {
        this->XoffsetPixels = 0;
        this->WidthPixels = 0;
        this->HeightPixels = 0;
        this->AdvanceWidthPixels = 0;
        this->LeftBearingPixels = 0;
        this->TopBearingPixels = 0;
    }
};
}
