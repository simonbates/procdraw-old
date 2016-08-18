#include "procdraw/utils/font_metrics.h"

namespace procdraw {

    void TextureFontMetrics::ClearGlyphs(int maxCharCode)
    {
        glyphMetrics_.clear();
        glyphMetrics_.resize(maxCharCode + 1);
    }

    int TextureFontMetrics::MaxCharCode()
    {
        return glyphMetrics_.size() - 1;
    }

    void TextureFontMetrics::SetGlyph(int charCode, const TextureGlyphMetrics &glyphMetrics)
    {
        glyphMetrics_.at(charCode) = glyphMetrics;
    }

    TextureGlyphMetrics& TextureFontMetrics::GetGlyph(int charCode)
    {
        return glyphMetrics_.at(charCode);
    }

}
