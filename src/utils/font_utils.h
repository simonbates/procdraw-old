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

    struct GlyphCoords {
        float Left;
        float Right;
        float Top;
        float Bottom;
        float TextureLeft;
        float TextureRight;
        float TextureTop;
        float TextureBottom;
    };

    template <typename T>
    class TextLayout {
    public:
        typedef typename std::vector<std::vector<T>>::size_type size_type;
        std::vector<T>& GetLineVertices(size_type pos);
        std::vector<T>& OpenNewLine();
        size_type Size();
    private:
        std::vector<std::vector<T>> lines_;
    };

    template <typename T>
    std::vector<T>& TextLayout<T>::GetLineVertices(size_type pos)
    {
        return lines_.at(pos);
    }

    template <typename T>
    std::vector<T>& TextLayout<T>::OpenNewLine()
    {
        lines_.resize(lines_.size() + 1);
        return lines_.back();
    }

    template <typename T>
    typename TextLayout<T>::size_type TextLayout<T>::Size()
    {
        return lines_.size();
    }

    void CalculateFixedWidthBlockCursorPos(int cursorTextPosition, int glyphWidth, int *x, int *width);

    GlyphCoords LayOutGlyph(const TextureGlyphMetrics &glyphMetrics, const TextureFontMetrics &fontMetrics);

    template <typename T>
    TextLayout<T> LayOutText(const std::string &text, TextureFontMetrics &fontMetrics, int maxDrawGlyphs)
    {
        TextLayout<T> layout;
        std::vector<T>& vertices = layout.OpenNewLine();

        int cursorX = 0;
        int numGlyphs = 0;
        int maxCharCode = fontMetrics.MaxCharCode();
        TextureGlyphMetrics glyphMetrics;
        GlyphCoords glyphCoords;

        for (const char &ch : text) {
            if (numGlyphs >= maxDrawGlyphs) {
                break;
            }

            if (ch <= 32 || ch > maxCharCode) {
                cursorX += fontMetrics.GetGlyph(32).AdvanceWidthPixels;
                continue;
            }

            glyphMetrics = fontMetrics.GetGlyph(ch);
            glyphCoords = LayOutGlyph(glyphMetrics, fontMetrics);

            // Left top
            vertices.push_back(cursorX + glyphCoords.Left);
            vertices.push_back(glyphCoords.Top);
            vertices.push_back(glyphCoords.TextureLeft);
            vertices.push_back(glyphCoords.TextureTop);
            // Left bottom
            vertices.push_back(cursorX + glyphCoords.Left);
            vertices.push_back(glyphCoords.Bottom);
            vertices.push_back(glyphCoords.TextureLeft);
            vertices.push_back(glyphCoords.TextureBottom);
            // Right top
            vertices.push_back(cursorX + glyphCoords.Right);
            vertices.push_back(glyphCoords.Top);
            vertices.push_back(glyphCoords.TextureRight);
            vertices.push_back(glyphCoords.TextureTop);

            // Left bottom
            vertices.push_back(cursorX + glyphCoords.Left);
            vertices.push_back(glyphCoords.Bottom);
            vertices.push_back(glyphCoords.TextureLeft);
            vertices.push_back(glyphCoords.TextureBottom);
            // Right bottom
            vertices.push_back(cursorX + glyphCoords.Right);
            vertices.push_back(glyphCoords.Bottom);
            vertices.push_back(glyphCoords.TextureRight);
            vertices.push_back(glyphCoords.TextureBottom);
            // Right top
            vertices.push_back(cursorX + glyphCoords.Right);
            vertices.push_back(glyphCoords.Top);
            vertices.push_back(glyphCoords.TextureRight);
            vertices.push_back(glyphCoords.TextureTop);

            cursorX += glyphMetrics.AdvanceWidthPixels;
            ++numGlyphs;
        }

        return layout;
    }

}
