#pragma once

#include "procdraw_core/font_metrics.h"
#include <GL/gl3w.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>

namespace procdraw {

const int FreeTypeFontLoaderMaxAsciiCode = 126;

class FreeTypeFontLoader
{
public:
  FreeTypeFontLoader();
  void LoadFont(const std::string& path, int sizePixels,
                BitmapFontMetrics* out_fontMetrics, GLuint* out_fontTexture);

private:
  FT_Library ft_;
  void MakeFontTexture(FT_Face face, FT_ULong fromCharCode, FT_ULong toCharCode,
                       BitmapFontMetrics* fontMetrics, GLuint* fontTexture);
  void CalculateTextureSize(FT_Face face, FT_ULong fromCharCode,
                            FT_ULong toCharCode, GLsizei* width,
                            GLsizei* height);
  void PopulateTexture(FT_Face face, FT_ULong fromCharCode, FT_ULong toCharCode,
                       BitmapFontMetrics* fontMetrics);
  void RenderChar(FT_Face face, FT_ULong charCode);
};
}
