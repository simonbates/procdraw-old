#pragma once

#include <vector>

namespace procdraw {

struct BitmapGlyphMetrics
{
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

class BitmapFontMetrics
{
public:
  // Distance from baseline to highest point in the face
  int AscenderPixels = 0;
  // Distance from baseline to lowest point in the face (negative)
  int DescenderPixels = 0;
  // Vertical distance between two consecutive baselines
  int LinespacePixels = 0;
  int BitmapWidth = 0;
  int BitmapHeight = 0;
  void ClearGlyphs(int maxCharCode);
  int MaxCharCode();
  void SetGlyph(int charCode, const BitmapGlyphMetrics& glyphMetrics);
  BitmapGlyphMetrics& GetGlyph(int charCode);

private:
  std::vector<BitmapGlyphMetrics> glyphMetrics_;
};
}
