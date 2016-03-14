#pragma once

#include <string>
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

    void CalculateFixedWidthBlockCursorPos(int cursorTextPosition, int glyphWidth, int *x, int *width);

    template <typename T>
    void LayOutText(const std::string &text, TextureFontMetrics &metrics, int maxDrawGlyphs, std::vector<T> &vertices)
    {
        vertices.clear();

        int cursorX = 0;
        int numGlyphs = 0;
        int maxCharCode = metrics.MaxCharCode();

        for (const char &ch : text) {
            if (numGlyphs >= maxDrawGlyphs) {
                break;
            }

            if (ch <= 32 || ch > maxCharCode) {
                cursorX += metrics.GetGlyph(32).AdvanceWidthPixels;
                continue;
            }

            unsigned int charCode = ch;

            float glyphWidth = metrics.GetGlyph(charCode).WidthPixels;
            float glyphHeight = metrics.GetGlyph(charCode).HeightPixels;

            float glyphLeft = cursorX + metrics.GetGlyph(charCode).LeftBearingPixels;
            float glyphRight = glyphLeft + glyphWidth;
            float glyphTop = metrics.AscenderPixels - metrics.GetGlyph(charCode).TopBearingPixels;
            float glyphBottom = glyphTop + glyphHeight;

            float glyphTextureLeft = ((float)metrics.GetGlyph(charCode).XoffsetPixels) / metrics.TextureWidth;
            float glyphTextureRight = (metrics.GetGlyph(charCode).XoffsetPixels + glyphWidth) / metrics.TextureWidth;
            float glyphTextureTop = 0.0f;
            float glyphTextureBottom = glyphHeight / metrics.TextureHeight;

            // Left top
            vertices.push_back(glyphLeft);
            vertices.push_back(glyphTop);
            vertices.push_back(glyphTextureLeft);
            vertices.push_back(glyphTextureTop);
            // Left bottom
            vertices.push_back(glyphLeft);
            vertices.push_back(glyphBottom);
            vertices.push_back(glyphTextureLeft);
            vertices.push_back(glyphTextureBottom);
            // Right top
            vertices.push_back(glyphRight);
            vertices.push_back(glyphTop);
            vertices.push_back(glyphTextureRight);
            vertices.push_back(glyphTextureTop);

            // Left bottom
            vertices.push_back(glyphLeft);
            vertices.push_back(glyphBottom);
            vertices.push_back(glyphTextureLeft);
            vertices.push_back(glyphTextureBottom);
            // Right bottom
            vertices.push_back(glyphRight);
            vertices.push_back(glyphBottom);
            vertices.push_back(glyphTextureRight);
            vertices.push_back(glyphTextureBottom);
            // Right top
            vertices.push_back(glyphRight);
            vertices.push_back(glyphTop);
            vertices.push_back(glyphTextureRight);
            vertices.push_back(glyphTextureTop);

            cursorX += metrics.GetGlyph(charCode).AdvanceWidthPixels;
            ++numGlyphs;
        }
    }

}
