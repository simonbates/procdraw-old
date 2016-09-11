#include "procdraw/graphics/freetype_font_loader.h"
#include "procdraw/utils/utils.h"
#include <stdexcept>

namespace procdraw {

FreeTypeFontLoader::FreeTypeFontLoader()
{
  if (FT_Init_FreeType(&ft_)) {
    throw std::runtime_error("Error FT_Init_FreeType");
  }
}

void
FreeTypeFontLoader::LoadFont(const std::string& path, int sizePixels,
                             BitmapFontMetrics* out_fontMetrics,
                             GLuint* out_fontTexture)
{
  FT_Face face;

  if (FT_New_Face(ft_, path.c_str(), 0, &face)) {
    throw std::runtime_error("Error loading font " + path);
  }

  FT_Set_Pixel_Sizes(face, 0, sizePixels);

  out_fontMetrics->AscenderPixels = face->size->metrics.ascender / 64;
  out_fontMetrics->DescenderPixels = face->size->metrics.descender / 64;
  out_fontMetrics->LinespacePixels = face->size->metrics.height / 64;

  MakeFontTexture(face, 32, FreeTypeFontLoaderMaxAsciiCode, out_fontMetrics,
                  out_fontTexture);
  PopulateTexture(face, 32, FreeTypeFontLoaderMaxAsciiCode, out_fontMetrics);
}

void
FreeTypeFontLoader::MakeFontTexture(FT_Face face, FT_ULong fromCharCode,
                                    FT_ULong toCharCode,
                                    BitmapFontMetrics* fontMetrics,
                                    GLuint* fontTexture)
{
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, fontTexture);
  glBindTexture(GL_TEXTURE_2D, *fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  CalculateTextureSize(face, fromCharCode, toCharCode,
                       &(fontMetrics->BitmapWidth),
                       &(fontMetrics->BitmapHeight));

  // Allocate the texture memory
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontMetrics->BitmapWidth,
               fontMetrics->BitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}

void
FreeTypeFontLoader::CalculateTextureSize(FT_Face face, FT_ULong fromCharCode,
                                         FT_ULong toCharCode, GLsizei* width,
                                         GLsizei* height)
{
  *width = 0;
  *height = 0;
  for (auto charCode = fromCharCode; charCode < toCharCode; ++charCode) {
    RenderChar(face, charCode);
    FT_GlyphSlot g = face->glyph;
    *width += g->bitmap.width;
    if (g->bitmap.rows > *height) {
      *height = g->bitmap.rows;
    }
  }
  // Ensure that the texture dimensions are powers of 2
  *width = PowerOf2Gte(*width);
  *height = PowerOf2Gte(*height);
}

void
FreeTypeFontLoader::PopulateTexture(FT_Face face, FT_ULong fromCharCode,
                                    FT_ULong toCharCode,
                                    BitmapFontMetrics* fontMetrics)
{
  fontMetrics->ClearGlyphs(toCharCode);

  GLint xoffset = 0;

  BitmapGlyphMetrics glyphMetrics;

  for (auto charCode = fromCharCode; charCode < toCharCode; ++charCode) {
    RenderChar(face, charCode);
    FT_GlyphSlot g = face->glyph;

    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, 0, g->bitmap.width,
                    g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

    glyphMetrics.XoffsetPixels = xoffset;
    glyphMetrics.WidthPixels = g->bitmap.width;
    glyphMetrics.HeightPixels = g->bitmap.rows;
    glyphMetrics.AdvanceWidthPixels = g->advance.x / 64;
    glyphMetrics.LeftBearingPixels = g->bitmap_left;
    glyphMetrics.TopBearingPixels = g->bitmap_top;

    fontMetrics->SetGlyph(charCode, glyphMetrics);

    xoffset += g->bitmap.width;
  }
}

void
FreeTypeFontLoader::RenderChar(FT_Face face, FT_ULong charCode)
{
  if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
    throw std::runtime_error("Error FT_Load_Char");
  }
}
}
