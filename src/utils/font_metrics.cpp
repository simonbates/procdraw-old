#include "procdraw/utils/font_metrics.h"

namespace procdraw {

void
BitmapFontMetrics::ClearGlyphs(int maxCharCode)
{
  glyphMetrics_.clear();
  glyphMetrics_.resize(maxCharCode + 1);
}

int
BitmapFontMetrics::MaxCharCode()
{
  return glyphMetrics_.size() - 1;
}

void
BitmapFontMetrics::SetGlyph(int charCode,
                            const BitmapGlyphMetrics& glyphMetrics)
{
  glyphMetrics_.at(charCode) = glyphMetrics;
}

BitmapGlyphMetrics&
BitmapFontMetrics::GetGlyph(int charCode)
{
  return glyphMetrics_.at(charCode);
}
}
