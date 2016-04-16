#include "font_utils.h"
#include "catch.hpp"

void MakeTestFontMetrics(procdraw::TextureFontMetrics &metrics)
{
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
}

void AssertGlyphVertices(const procdraw::GlyphCoords &expected, float expectedHorizontalOffset,
                         const std::vector<float> &vertices, int verticesOffset)
{
    // Left top
    REQUIRE(vertices[verticesOffset + 0] == expectedHorizontalOffset + expected.Left);
    REQUIRE(vertices[verticesOffset + 1] == expected.Top);
    REQUIRE(vertices[verticesOffset + 2] == expected.TextureLeft);
    REQUIRE(vertices[verticesOffset + 3] == expected.TextureTop);

    // Left bottom
    REQUIRE(vertices[verticesOffset + 4] == expectedHorizontalOffset + expected.Left);
    REQUIRE(vertices[verticesOffset + 5] == expected.Bottom);
    REQUIRE(vertices[verticesOffset + 6] == expected.TextureLeft);
    REQUIRE(vertices[verticesOffset + 7] == expected.TextureBottom);

    // Right top
    REQUIRE(vertices[verticesOffset + 8] == expectedHorizontalOffset + expected.Right);
    REQUIRE(vertices[verticesOffset + 9] == expected.Top);
    REQUIRE(vertices[verticesOffset + 10] == expected.TextureRight);
    REQUIRE(vertices[verticesOffset + 11] == expected.TextureTop);

    // Left bottom
    REQUIRE(vertices[verticesOffset + 12] == expectedHorizontalOffset + expected.Left);
    REQUIRE(vertices[verticesOffset + 13] == expected.Bottom);
    REQUIRE(vertices[verticesOffset + 14] == expected.TextureLeft);
    REQUIRE(vertices[verticesOffset + 15] == expected.TextureBottom);

    // Right bottom
    REQUIRE(vertices[verticesOffset + 16] == expectedHorizontalOffset + expected.Right);
    REQUIRE(vertices[verticesOffset + 17] == expected.Bottom);
    REQUIRE(vertices[verticesOffset + 18] == expected.TextureRight);
    REQUIRE(vertices[verticesOffset + 19] == expected.TextureBottom);

    // Right top
    REQUIRE(vertices[verticesOffset + 20] == expectedHorizontalOffset + expected.Right);
    REQUIRE(vertices[verticesOffset + 21] == expected.Top);
    REQUIRE(vertices[verticesOffset + 22] == expected.TextureRight);
    REQUIRE(vertices[verticesOffset + 23] == expected.TextureTop);
}

TEST_CASE("Font utils")
{

    procdraw::TextureFontMetrics fontMetrics;
    MakeTestFontMetrics(fontMetrics);

    procdraw::GlyphCoords exclamationCoords = procdraw::LayOutGlyph(fontMetrics.GetGlyph(33), fontMetrics);
    procdraw::GlyphCoords quoteCoords = procdraw::LayOutGlyph(fontMetrics.GetGlyph(34), fontMetrics);

    SECTION("CalculateFixedWidthBlockCursorPos") {
        int x, width;

        procdraw::CalculateFixedWidthBlockCursorPos(0, 10, &x, &width);
        REQUIRE(x == 0);
        REQUIRE(width == 10);

        procdraw::CalculateFixedWidthBlockCursorPos(2, 10, &x, &width);
        REQUIRE(x == 20);
        REQUIRE(width == 10);
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

        SECTION("Text is laid out") {
            procdraw::TextLayout<float> layout = procdraw::LayOutText<float>(" ! \"", fontMetrics, 10);

            REQUIRE(layout.Size() == 1);

            auto vertices = layout.GetLineVertices(0);
            REQUIRE(vertices.size() == 24 * 2);
            AssertGlyphVertices(exclamationCoords, spaceAdvance, vertices, 0);
            AssertGlyphVertices(quoteCoords, spaceAdvance * 2 + exclamationAdvance, vertices, 24);
        }

    }

}
