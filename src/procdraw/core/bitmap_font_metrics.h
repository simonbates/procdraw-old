#pragma once

#include "procdraw/core/bitmap_glyph_metrics.h"
#include <vector>

namespace procdraw {

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
  int MaxCharCode() const;
  void SetGlyph(int charCode, const BitmapGlyphMetrics& glyphMetrics);
  BitmapGlyphMetrics GetGlyph(int charCode) const;

private:
  std::vector<BitmapGlyphMetrics> glyphMetrics_;
};
}
