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
        void OpenNewLine();
        size_type NumLines() const;
        void AddGlyph(const GlyphCoords &glyphCoords, int cursorX, int cursorY);
        const std::vector<T>& GetVerticesForLine(size_type lineNum) const;
        void SetNumCharsInLine(int n);
        int GetNumCharsInLine(size_type lineNum);
    private:
        std::vector<std::vector<T>> lines_;
        std::vector<int> numChars_;
    };

    template <typename T>
    void TextLayout<T>::OpenNewLine()
    {
        lines_.resize(lines_.size() + 1);
        numChars_.push_back(0);
    }

    template <typename T>
    typename TextLayout<T>::size_type TextLayout<T>::NumLines() const
    {
        return lines_.size();
    }

    template <typename T>
    void TextLayout<T>::AddGlyph(const GlyphCoords &glyphCoords, int cursorX, int cursorY)
    {
        if (!lines_.empty()) {
            std::vector<T> &vertices = lines_.back();

            // Left top
            vertices.push_back(cursorX + glyphCoords.Left);
            vertices.push_back(cursorY + glyphCoords.Top);
            vertices.push_back(glyphCoords.TextureLeft);
            vertices.push_back(glyphCoords.TextureTop);
            // Left bottom
            vertices.push_back(cursorX + glyphCoords.Left);
            vertices.push_back(cursorY + glyphCoords.Bottom);
            vertices.push_back(glyphCoords.TextureLeft);
            vertices.push_back(glyphCoords.TextureBottom);
            // Right top
            vertices.push_back(cursorX + glyphCoords.Right);
            vertices.push_back(cursorY + glyphCoords.Top);
            vertices.push_back(glyphCoords.TextureRight);
            vertices.push_back(glyphCoords.TextureTop);

            // Left bottom
            vertices.push_back(cursorX + glyphCoords.Left);
            vertices.push_back(cursorY + glyphCoords.Bottom);
            vertices.push_back(glyphCoords.TextureLeft);
            vertices.push_back(glyphCoords.TextureBottom);
            // Right bottom
            vertices.push_back(cursorX + glyphCoords.Right);
            vertices.push_back(cursorY + glyphCoords.Bottom);
            vertices.push_back(glyphCoords.TextureRight);
            vertices.push_back(glyphCoords.TextureBottom);
            // Right top
            vertices.push_back(cursorX + glyphCoords.Right);
            vertices.push_back(cursorY + glyphCoords.Top);
            vertices.push_back(glyphCoords.TextureRight);
            vertices.push_back(glyphCoords.TextureTop);
        }
    }

    template <typename T>
    const std::vector<T>& TextLayout<T>::GetVerticesForLine(size_type lineNum) const
    {
        return lines_.at(lineNum);
    }

    template <typename T>
    void TextLayout<T>::SetNumCharsInLine(int n)
    {
        if (!numChars_.empty()) {
            numChars_.back() = n;
        }
    }

    template <typename T>
    int TextLayout<T>::GetNumCharsInLine(size_type lineNum)
    {
        return numChars_.at(lineNum);
    }

    void CalculateFixedWidthBlockCursorPos(int cursorTextPosition, int glyphWidth, int *x, int *width);

    GlyphCoords LayOutGlyph(const TextureGlyphMetrics &glyphMetrics, const TextureFontMetrics &fontMetrics);

    template <typename T>
    TextLayout<T> LayOutText(const std::string &text, TextureFontMetrics &fontMetrics,
                             int maxDrawGlyphsPerLine, int maxLineWidthPixels)
    {
        TextLayout<T> layout;
        layout.OpenNewLine();

        int cursorX = 0;
        int cursorY = 0;
        int numGlyphsThisLine = 0;
        int numCharsThisLine = 0;
        int maxCharCode = fontMetrics.MaxCharCode();
        TextureGlyphMetrics glyphMetrics;
        GlyphCoords glyphCoords;
        int spaceAdvance = fontMetrics.GetGlyph(32).AdvanceWidthPixels;

        for (const char &ch : text) {
            if (ch <= 32 || ch > maxCharCode) {
                if (cursorX + spaceAdvance > maxLineWidthPixels) {
                    layout.SetNumCharsInLine(numCharsThisLine);
                    layout.OpenNewLine();
                    cursorX = 0;
                    cursorY += fontMetrics.LinespacePixels;
                    numGlyphsThisLine = 0;
                    numCharsThisLine = 0;
                }
                ++numCharsThisLine;
                cursorX += spaceAdvance;
            }
            else {
                glyphMetrics = fontMetrics.GetGlyph(ch);
                glyphCoords = LayOutGlyph(glyphMetrics, fontMetrics);

                if ((cursorX + glyphMetrics.AdvanceWidthPixels > maxLineWidthPixels)
                        || (numGlyphsThisLine >= maxDrawGlyphsPerLine)) {
                    layout.SetNumCharsInLine(numCharsThisLine);
                    layout.OpenNewLine();
                    cursorX = 0;
                    cursorY += fontMetrics.LinespacePixels;
                    numGlyphsThisLine = 0;
                    numCharsThisLine = 0;
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

}
