#include "font_utils.h"
#include "catch.hpp"

procdraw::TextureFontMetrics MakeTestFontMetrics()
{
    procdraw::TextureFontMetrics metrics;

    metrics.AscenderPixels = 32;
    metrics.DescenderPixels = -8;
    metrics.LinespacePixels = 48;
    metrics.TextureWidth = 128;
    metrics.TextureHeight = 64;

    metrics.ClearGlyphs(34);

    procdraw::TextureGlyphMetrics space;
    space.XoffsetPixels = 0;
    space.WidthPixels = 0;
    space.HeightPixels = 0;
    space.AdvanceWidthPixels = 30;
    space.LeftBearingPixels = 0;
    space.TopBearingPixels = 0;
    metrics.SetGlyph(32, space);

    procdraw::TextureGlyphMetrics exclamation;
    exclamation.XoffsetPixels = 0;
    exclamation.WidthPixels = 14;
    exclamation.HeightPixels = 28;
    exclamation.AdvanceWidthPixels = 18;
    exclamation.LeftBearingPixels = 2;
    exclamation.TopBearingPixels = 26;
    metrics.SetGlyph(33, exclamation);

    procdraw::TextureGlyphMetrics quote;
    quote.XoffsetPixels = 32;
    quote.WidthPixels = 16;
    quote.HeightPixels = 12;
    quote.AdvanceWidthPixels = 24;
    quote.LeftBearingPixels = 4;
    quote.TopBearingPixels = 20;
    metrics.SetGlyph(34, quote);

    return metrics;
}

void AssertGlyphVertices(const procdraw::GlyphCoords &expected,
                         float expectedHorizontalOffset, float expectedVerticalOffset,
                         const std::vector<float> &vertices, int verticesOffset)
{
    // Left top
    REQUIRE(vertices[verticesOffset + 0] == expectedHorizontalOffset + expected.Left);
    REQUIRE(vertices[verticesOffset + 1] == expectedVerticalOffset + expected.Top);
    REQUIRE(vertices[verticesOffset + 2] == expected.TextureLeft);
    REQUIRE(vertices[verticesOffset + 3] == expected.TextureTop);

    // Left bottom
    REQUIRE(vertices[verticesOffset + 4] == expectedHorizontalOffset + expected.Left);
    REQUIRE(vertices[verticesOffset + 5] == expectedVerticalOffset + expected.Bottom);
    REQUIRE(vertices[verticesOffset + 6] == expected.TextureLeft);
    REQUIRE(vertices[verticesOffset + 7] == expected.TextureBottom);

    // Right top
    REQUIRE(vertices[verticesOffset + 8] == expectedHorizontalOffset + expected.Right);
    REQUIRE(vertices[verticesOffset + 9] == expectedVerticalOffset + expected.Top);
    REQUIRE(vertices[verticesOffset + 10] == expected.TextureRight);
    REQUIRE(vertices[verticesOffset + 11] == expected.TextureTop);

    // Left bottom
    REQUIRE(vertices[verticesOffset + 12] == expectedHorizontalOffset + expected.Left);
    REQUIRE(vertices[verticesOffset + 13] == expectedVerticalOffset + expected.Bottom);
    REQUIRE(vertices[verticesOffset + 14] == expected.TextureLeft);
    REQUIRE(vertices[verticesOffset + 15] == expected.TextureBottom);

    // Right bottom
    REQUIRE(vertices[verticesOffset + 16] == expectedHorizontalOffset + expected.Right);
    REQUIRE(vertices[verticesOffset + 17] == expectedVerticalOffset + expected.Bottom);
    REQUIRE(vertices[verticesOffset + 18] == expected.TextureRight);
    REQUIRE(vertices[verticesOffset + 19] == expected.TextureBottom);

    // Right top
    REQUIRE(vertices[verticesOffset + 20] == expectedHorizontalOffset + expected.Right);
    REQUIRE(vertices[verticesOffset + 21] == expectedVerticalOffset + expected.Top);
    REQUIRE(vertices[verticesOffset + 22] == expected.TextureRight);
    REQUIRE(vertices[verticesOffset + 23] == expected.TextureTop);
}

TEST_CASE("Font utils")
{

    procdraw::TextureFontMetrics fontMetrics = MakeTestFontMetrics();

    procdraw::GlyphCoords exclamationCoords = procdraw::LayOutGlyph(fontMetrics.GetGlyph(33), fontMetrics);
    procdraw::GlyphCoords quoteCoords = procdraw::LayOutGlyph(fontMetrics.GetGlyph(34), fontMetrics);

    SECTION("CalculateFixedWidthBlockCursorPos") {
        int x, y, width, height;

        procdraw::TextLayout<float> layout;
        layout.FixedGlyphWidth = 10;
        layout.GlyphHeight = 14;
        layout.LinespacePixels = 16;
        layout.MaxLineWidthPixels = 100;

        SECTION("Cursor is positioned at 0,0 for an empty layout") {
            layout.CalculateFixedWidthBlockCursorPos(0, x, y, width, height);
            REQUIRE(x == 0);
            REQUIRE(y == 0);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }

        SECTION("Cursor is positioned at 0,0 for a layout with a single empty line") {
            layout.OpenNewLine();
            layout.CalculateFixedWidthBlockCursorPos(0, x, y, width, height);
            REQUIRE(x == 0);
            REQUIRE(y == 0);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }

        SECTION("Cursor is positioned at the end of a single line") {
            layout.OpenNewLine();
            layout.SetNumCharsInLine(8);
            layout.CalculateFixedWidthBlockCursorPos(8, x, y, width, height);
            REQUIRE(x == 80);
            REQUIRE(y == 0);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }

        SECTION("Cursor wraps when exceeds maxLineWidthPixels") {
            layout.MaxLineWidthPixels = 89;
            layout.OpenNewLine();
            layout.SetNumCharsInLine(8);
            layout.CalculateFixedWidthBlockCursorPos(8, x, y, width, height);
            REQUIRE(x == 0);
            REQUIRE(y == 16);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }

        SECTION("Cursor does not wrap when positioned right at maxLineWidthPixels") {
            layout.MaxLineWidthPixels = 90;
            layout.OpenNewLine();
            layout.SetNumCharsInLine(8);
            layout.CalculateFixedWidthBlockCursorPos(8, x, y, width, height);
            REQUIRE(x == 80);
            REQUIRE(y == 0);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }

        SECTION("Cursor is positioned on a second line") {
            layout.OpenNewLine();
            layout.SetNumCharsInLine(8);
            layout.OpenNewLine();
            layout.SetNumCharsInLine(4);
            layout.CalculateFixedWidthBlockCursorPos(10, x, y, width, height);
            REQUIRE(x == 20);
            REQUIRE(y == 16);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }

        SECTION("Cursor is positioned beyond the char count") {
            layout.OpenNewLine();
            layout.SetNumCharsInLine(8);
            layout.CalculateFixedWidthBlockCursorPos(100, x, y, width, height);
            REQUIRE(x == 0);
            REQUIRE(y == 32);
            REQUIRE(width == 10);
            REQUIRE(height == 14);
        }
    }

    SECTION("LayOutGlyph") {
        REQUIRE(exclamationCoords.Left == 2);
        REQUIRE(exclamationCoords.Right == 16);
        REQUIRE(exclamationCoords.Top == 6);
        REQUIRE(exclamationCoords.Bottom == 34);
        REQUIRE(exclamationCoords.TextureLeft == 0.0f);
        REQUIRE(exclamationCoords.TextureRight == 0.109375f);
        REQUIRE(exclamationCoords.TextureTop == 0.0f);
        REQUIRE(exclamationCoords.TextureBottom == 0.4375f);

        REQUIRE(quoteCoords.Left == 4);
        REQUIRE(quoteCoords.Right == 20);
        REQUIRE(quoteCoords.Top == 12);
        REQUIRE(quoteCoords.Bottom == 24);
        REQUIRE(quoteCoords.TextureLeft == 0.25f);
        REQUIRE(quoteCoords.TextureRight == 0.375f);
        REQUIRE(quoteCoords.TextureTop == 0.0f);
        REQUIRE(quoteCoords.TextureBottom == 0.1875f);
    }

    SECTION("LayOutText") {

        float spaceAdvance = fontMetrics.GetGlyph(32).AdvanceWidthPixels;
        float exclamationAdvance = fontMetrics.GetGlyph(33).AdvanceWidthPixels;
        float quoteAdvance = fontMetrics.GetGlyph(34).AdvanceWidthPixels;

        SECTION("A layout for an empty string has one empty line") {
            auto layout = procdraw::LayOutText<float>("", fontMetrics, 100, 100);

            REQUIRE(layout.FixedGlyphWidth == spaceAdvance);
            REQUIRE(layout.GlyphHeight == 40);
            REQUIRE(layout.LinespacePixels == fontMetrics.LinespacePixels);
            REQUIRE(layout.MaxLineWidthPixels == 100);

            REQUIRE(layout.NumLines() == 1);
            REQUIRE(layout.GetNumCharsInLine(0) == 0);
            REQUIRE(layout.GetVerticesForLine(0).size() == 0);
        }

        SECTION("A single line of text is laid out") {
            auto layout = procdraw::LayOutText<float>(" ! \"", fontMetrics, 10, 1000);

            REQUIRE(layout.FixedGlyphWidth == spaceAdvance);
            REQUIRE(layout.GlyphHeight == 40);
            REQUIRE(layout.LinespacePixels == fontMetrics.LinespacePixels);
            REQUIRE(layout.MaxLineWidthPixels == 1000);

            REQUIRE(layout.NumLines() == 1);

            REQUIRE(layout.GetNumCharsInLine(0) == 4);

            auto vertices = layout.GetVerticesForLine(0);
            REQUIRE(vertices.size() == 24 * 2);
            AssertGlyphVertices(exclamationCoords, spaceAdvance, 0, vertices, 0);
            AssertGlyphVertices(quoteCoords, spaceAdvance * 2 + exclamationAdvance, 0, vertices, 24);
        }

        SECTION("Text is wrapped when the max number of glyphs is exceeded") {
            auto layout = procdraw::LayOutText<float>(" !!!!\"\"", fontMetrics, 4, 1000);

            // Expect:
            //
            // ' !!!!'
            // '""'

            REQUIRE(layout.FixedGlyphWidth == spaceAdvance);
            REQUIRE(layout.GlyphHeight == 40);
            REQUIRE(layout.LinespacePixels == fontMetrics.LinespacePixels);
            REQUIRE(layout.MaxLineWidthPixels == 1000);

            REQUIRE(layout.NumLines() == 2);

            // Number of chars in each line

            REQUIRE(layout.GetNumCharsInLine(0) == 5);
            REQUIRE(layout.GetNumCharsInLine(1) == 2);

            // Vertices

            auto lineOneVertices = layout.GetVerticesForLine(0);
            REQUIRE(lineOneVertices.size() == 24 * 4);
            AssertGlyphVertices(exclamationCoords, spaceAdvance + exclamationAdvance * 0, 0, lineOneVertices, 24 * 0);
            AssertGlyphVertices(exclamationCoords, spaceAdvance + exclamationAdvance * 1, 0, lineOneVertices, 24 * 1);
            AssertGlyphVertices(exclamationCoords, spaceAdvance + exclamationAdvance * 2, 0, lineOneVertices, 24 * 2);
            AssertGlyphVertices(exclamationCoords, spaceAdvance + exclamationAdvance * 3, 0, lineOneVertices, 24 * 3);

            auto lineTwoVertices = layout.GetVerticesForLine(1);
            REQUIRE(lineTwoVertices.size() == 24 * 2);
            AssertGlyphVertices(quoteCoords, quoteAdvance * 0, fontMetrics.LinespacePixels, lineTwoVertices, 24 * 0);
            AssertGlyphVertices(quoteCoords, quoteAdvance * 1, fontMetrics.LinespacePixels, lineTwoVertices, 24 * 1);
        }

        SECTION("Text is wrapped when the max pixel width is exceeded") {
            auto layout = procdraw::LayOutText<float>("    ! !!!!!!", fontMetrics, 1000, spaceAdvance * 3);

            // Expect:
            //
            // '   '
            // ' ! '
            // '!!!!!'
            // '!'

            REQUIRE(layout.FixedGlyphWidth == spaceAdvance);
            REQUIRE(layout.GlyphHeight == 40);
            REQUIRE(layout.LinespacePixels == fontMetrics.LinespacePixels);
            REQUIRE(layout.MaxLineWidthPixels == spaceAdvance * 3);

            REQUIRE(layout.NumLines() == 4);

            // Number of chars in each line

            REQUIRE(layout.GetNumCharsInLine(0) == 3);
            REQUIRE(layout.GetNumCharsInLine(1) == 3);
            REQUIRE(layout.GetNumCharsInLine(2) == 5);
            REQUIRE(layout.GetNumCharsInLine(3) == 1);

            // Vertices

            REQUIRE(layout.GetVerticesForLine(0).size() == 0);

            auto lineTwoVertices = layout.GetVerticesForLine(1);
            REQUIRE(lineTwoVertices.size() == 24);
            AssertGlyphVertices(exclamationCoords, spaceAdvance, fontMetrics.LinespacePixels, lineTwoVertices, 0);

            auto lineThreeVertices = layout.GetVerticesForLine(2);
            REQUIRE(lineThreeVertices.size() == 24 * 5);
            AssertGlyphVertices(exclamationCoords, exclamationAdvance * 0, fontMetrics.LinespacePixels * 2,
                                lineThreeVertices, 24 * 0);
            AssertGlyphVertices(exclamationCoords, exclamationAdvance * 1, fontMetrics.LinespacePixels * 2,
                                lineThreeVertices, 24 * 1);
            AssertGlyphVertices(exclamationCoords, exclamationAdvance * 2, fontMetrics.LinespacePixels * 2,
                                lineThreeVertices, 24 * 2);
            AssertGlyphVertices(exclamationCoords, exclamationAdvance * 3, fontMetrics.LinespacePixels * 2,
                                lineThreeVertices, 24 * 3);
            AssertGlyphVertices(exclamationCoords, exclamationAdvance * 4, fontMetrics.LinespacePixels * 2,
                                lineThreeVertices, 24 * 4);

            auto lineFourVertices = layout.GetVerticesForLine(3);
            REQUIRE(lineFourVertices.size() == 24);
            AssertGlyphVertices(exclamationCoords, 0, fontMetrics.LinespacePixels * 3, lineFourVertices, 0);
        }

    }

}
