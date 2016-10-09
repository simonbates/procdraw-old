#pragma once

#include "procdraw/font_metrics.h"
#include "procdraw/text_layout.h"
#include <string>

namespace procdraw {

template <typename T>
class TextLayoutEngine
{
public:
  TextLayout<T> LayOutText(const std::string& text,
                           const BitmapFontMetrics& fontMetrics,
                           int maxDrawGlyphsPerLine, int maxLineWidthPixels);
  GlyphCoords LayOutGlyph(const procdraw::BitmapGlyphMetrics& glyphMetrics,
                          const procdraw::BitmapFontMetrics& fontMetrics);

private:
  int cursorX;
  int cursorY;
  int numGlyphsThisLine;
  int numCharsThisLine;
  void NewLine(TextLayout<T>* layout);
};

template <typename T>
TextLayout<T>
TextLayoutEngine<T>::LayOutText(const std::string& text,
                                const BitmapFontMetrics& fontMetrics,
                                int maxDrawGlyphsPerLine,
                                int maxLineWidthPixels)
{
  TextLayout<T> layout;

  layout.FixedGlyphWidth = fontMetrics.GetGlyph(32).AdvanceWidthPixels;
  layout.GlyphHeight = fontMetrics.AscenderPixels - fontMetrics.DescenderPixels;
  layout.LinespacePixels = fontMetrics.LinespacePixels;
  layout.MaxLineWidthPixels = maxLineWidthPixels;

  layout.OpenNewLine();

  cursorX = 0;
  cursorY = 0;
  numGlyphsThisLine = 0;
  numCharsThisLine = 0;

  int maxCharCode = fontMetrics.MaxCharCode();
  BitmapGlyphMetrics glyphMetrics;
  GlyphCoords glyphCoords;
  int spaceAdvance = fontMetrics.GetGlyph(32).AdvanceWidthPixels;

  for (const char& ch : text) {
    if (ch == '\n') {
      NewLine(&layout);
    } else if (ch <= 32 || ch > maxCharCode) {
      if (cursorX + spaceAdvance > maxLineWidthPixels) {
        NewLine(&layout);
      }
      ++numCharsThisLine;
      cursorX += spaceAdvance;
    } else {
      glyphMetrics = fontMetrics.GetGlyph(ch);
      glyphCoords = LayOutGlyph(glyphMetrics, fontMetrics);

      if ((cursorX + glyphMetrics.AdvanceWidthPixels > maxLineWidthPixels) ||
          (numGlyphsThisLine >= maxDrawGlyphsPerLine)) {
        NewLine(&layout);
      }

      layout.AddGlyph(glyphCoords, cursorX, cursorY);
      ++numGlyphsThisLine;
      ++numCharsThisLine;
      cursorX += glyphMetrics.AdvanceWidthPixels;
    }
  }

  layout.SetNumCharsInLine(numCharsThisLine);

  return layout;
}

template <typename T>
GlyphCoords
TextLayoutEngine<T>::LayOutGlyph(
  const procdraw::BitmapGlyphMetrics& glyphMetrics,
  const procdraw::BitmapFontMetrics& fontMetrics)
{
  GlyphCoords coords;
  coords.Left = static_cast<float>(glyphMetrics.LeftBearingPixels);
  coords.Right = coords.Left + glyphMetrics.WidthPixels;
  coords.Top = static_cast<float>(fontMetrics.AscenderPixels -
                                  glyphMetrics.TopBearingPixels);
  coords.Bottom = coords.Top + glyphMetrics.HeightPixels;
  coords.TextureLeft =
    static_cast<float>(glyphMetrics.XoffsetPixels) / fontMetrics.BitmapWidth;
  coords.TextureRight = (static_cast<float>(glyphMetrics.XoffsetPixels) +
                         glyphMetrics.WidthPixels) /
                        fontMetrics.BitmapWidth;
  coords.TextureTop = 0.0f;
  coords.TextureBottom =
    static_cast<float>(glyphMetrics.HeightPixels) / fontMetrics.BitmapHeight;
  return coords;
}

template <typename T>
void
TextLayoutEngine<T>::NewLine(TextLayout<T>* layout)
{
  layout->SetNumCharsInLine(numCharsThisLine);
  layout->OpenNewLine();
  cursorX = 0;
  cursorY += layout->LinespacePixels;
  numGlyphsThisLine = 0;
  numCharsThisLine = 0;
}
}
