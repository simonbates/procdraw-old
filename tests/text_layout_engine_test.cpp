#include "procdraw/text_layout_engine.h"
#include "gtest/gtest.h"
#include <sstream>
#include <vector>

class TextLayoutEngineTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        fontMetrics_ = MakeTestFontMetrics();
        exclamationCoords_ = layoutEngine_.LayOutGlyph(
            fontMetrics_.GetGlyph(33), fontMetrics_);
        quoteCoords_ = layoutEngine_.LayOutGlyph(
            fontMetrics_.GetGlyph(34), fontMetrics_);
        spaceAdvance_ = fontMetrics_.GetGlyph(32).AdvanceWidthPixels;
        exclamationAdvance_ = fontMetrics_.GetGlyph(33).AdvanceWidthPixels;
        quoteAdvance_ = fontMetrics_.GetGlyph(34).AdvanceWidthPixels;
        fontGlyphHeight_
            = fontMetrics_.AscenderPixels - fontMetrics_.DescenderPixels;
    }

    static procdraw::BitmapFontMetrics MakeTestFontMetrics()
    {
        procdraw::BitmapFontMetrics metrics;

        metrics.AscenderPixels = 32;
        metrics.DescenderPixels = -8;
        metrics.LinespacePixels = 48;
        metrics.BitmapWidth = 128;
        metrics.BitmapHeight = 64;

        metrics.ClearGlyphs(34);

        procdraw::BitmapGlyphMetrics space;
        space.XoffsetPixels = 0;
        space.WidthPixels = 0;
        space.HeightPixels = 0;
        space.AdvanceWidthPixels = 30;
        space.LeftBearingPixels = 0;
        space.TopBearingPixels = 0;
        metrics.SetGlyph(32, space);

        procdraw::BitmapGlyphMetrics exclamation;
        exclamation.XoffsetPixels = 0;
        exclamation.WidthPixels = 14;
        exclamation.HeightPixels = 28;
        exclamation.AdvanceWidthPixels = 18;
        exclamation.LeftBearingPixels = 2;
        exclamation.TopBearingPixels = 26;
        metrics.SetGlyph(33, exclamation);

        procdraw::BitmapGlyphMetrics quote;
        quote.XoffsetPixels = 32;
        quote.WidthPixels = 16;
        quote.HeightPixels = 12;
        quote.AdvanceWidthPixels = 24;
        quote.LeftBearingPixels = 4;
        quote.TopBearingPixels = 20;
        metrics.SetGlyph(34, quote);

        return metrics;
    }

    static void ExpectGlyphVertices(const procdraw::GlyphCoords& expected,
        int expectedHOffset, int expectedVOffset,
        const std::vector<float>& vertices, int verticesOffset)
    {

        std::ostringstream message;
        message << "verticesOffset=" << verticesOffset;
        SCOPED_TRACE(message.str());

        // Left top
        EXPECT_EQ(
            expectedHOffset + expected.Left, vertices[verticesOffset + 0]);
        EXPECT_EQ(expectedVOffset + expected.Top, vertices[verticesOffset + 1]);
        EXPECT_EQ(expected.TextureLeft, vertices[verticesOffset + 2]);
        EXPECT_EQ(expected.TextureTop, vertices[verticesOffset + 3]);

        // Left bottom
        EXPECT_EQ(
            expectedHOffset + expected.Left, vertices[verticesOffset + 4]);
        EXPECT_EQ(
            expectedVOffset + expected.Bottom, vertices[verticesOffset + 5]);
        EXPECT_EQ(expected.TextureLeft, vertices[verticesOffset + 6]);
        EXPECT_EQ(expected.TextureBottom, vertices[verticesOffset + 7]);

        // Right top
        EXPECT_EQ(
            expectedHOffset + expected.Right, vertices[verticesOffset + 8]);
        EXPECT_EQ(expectedVOffset + expected.Top, vertices[verticesOffset + 9]);
        EXPECT_EQ(expected.TextureRight, vertices[verticesOffset + 10]);
        EXPECT_EQ(expected.TextureTop, vertices[verticesOffset + 11]);

        // Left bottom
        EXPECT_EQ(
            expectedHOffset + expected.Left, vertices[verticesOffset + 12]);
        EXPECT_EQ(
            expectedVOffset + expected.Bottom, vertices[verticesOffset + 13]);
        EXPECT_EQ(expected.TextureLeft, vertices[verticesOffset + 14]);
        EXPECT_EQ(expected.TextureBottom, vertices[verticesOffset + 15]);

        // Right bottom
        EXPECT_EQ(
            expectedHOffset + expected.Right, vertices[verticesOffset + 16]);
        EXPECT_EQ(
            expectedVOffset + expected.Bottom, vertices[verticesOffset + 17]);
        EXPECT_EQ(expected.TextureRight, vertices[verticesOffset + 18]);
        EXPECT_EQ(expected.TextureBottom, vertices[verticesOffset + 19]);

        // Right top
        EXPECT_EQ(
            expectedHOffset + expected.Right, vertices[verticesOffset + 20]);
        EXPECT_EQ(
            expectedVOffset + expected.Top, vertices[verticesOffset + 21]);
        EXPECT_EQ(expected.TextureRight, vertices[verticesOffset + 22]);
        EXPECT_EQ(expected.TextureTop, vertices[verticesOffset + 23]);
    }

    procdraw::TextLayoutEngine<float> layoutEngine_;
    procdraw::BitmapFontMetrics fontMetrics_;
    procdraw::GlyphCoords exclamationCoords_;
    procdraw::GlyphCoords quoteCoords_;
    int spaceAdvance_;
    int exclamationAdvance_;
    int quoteAdvance_;
    int fontGlyphHeight_;
};

TEST_F(TextLayoutEngineTest, LayOutGlyph)
{
    EXPECT_EQ(2, exclamationCoords_.Left);
    EXPECT_EQ(16, exclamationCoords_.Right);
    EXPECT_EQ(6, exclamationCoords_.Top);
    EXPECT_EQ(34, exclamationCoords_.Bottom);
    EXPECT_EQ(0.0f, exclamationCoords_.TextureLeft);
    EXPECT_EQ(0.109375f, exclamationCoords_.TextureRight);
    EXPECT_EQ(0.0f, exclamationCoords_.TextureTop);
    EXPECT_EQ(0.4375f, exclamationCoords_.TextureBottom);

    EXPECT_EQ(4, quoteCoords_.Left);
    EXPECT_EQ(20, quoteCoords_.Right);
    EXPECT_EQ(12, quoteCoords_.Top);
    EXPECT_EQ(24, quoteCoords_.Bottom);
    EXPECT_EQ(0.25f, quoteCoords_.TextureLeft);
    EXPECT_EQ(0.375f, quoteCoords_.TextureRight);
    EXPECT_EQ(0.0f, quoteCoords_.TextureTop);
    EXPECT_EQ(0.1875f, quoteCoords_.TextureBottom);
}

TEST_F(TextLayoutEngineTest, LayOutTextEmptyStringHasOneEmptyLine)
{
    auto layout = layoutEngine_.LayOutText("", fontMetrics_, 100, 100);

    EXPECT_EQ(spaceAdvance_, layout.FixedGlyphWidth);
    EXPECT_EQ(fontGlyphHeight_, layout.GlyphHeight);
    EXPECT_EQ(fontMetrics_.LinespacePixels, layout.LinespacePixels);
    EXPECT_EQ(100, layout.MaxLineWidthPixels);

    EXPECT_EQ(static_cast<size_t>(1), layout.NumLines());
    EXPECT_EQ(0, layout.GetNumCharsInLine(0));
    EXPECT_EQ(static_cast<size_t>(0), layout.GetVerticesForLine(0).size());
}

TEST_F(TextLayoutEngineTest, LayOutTextSingleLine)
{
    auto layout = layoutEngine_.LayOutText(" ! \"", fontMetrics_, 10, 1000);

    EXPECT_EQ(spaceAdvance_, layout.FixedGlyphWidth);
    EXPECT_EQ(fontGlyphHeight_, layout.GlyphHeight);
    EXPECT_EQ(fontMetrics_.LinespacePixels, layout.LinespacePixels);
    EXPECT_EQ(1000, layout.MaxLineWidthPixels);

    EXPECT_EQ(static_cast<size_t>(1), layout.NumLines());

    EXPECT_EQ(4, layout.GetNumCharsInLine(0));

    auto vertices = layout.GetVerticesForLine(0);
    EXPECT_EQ(static_cast<size_t>(24 * 2), vertices.size());
    ExpectGlyphVertices(exclamationCoords_, spaceAdvance_, 0, vertices, 0);
    ExpectGlyphVertices(
        quoteCoords_, spaceAdvance_ * 2 + exclamationAdvance_, 0, vertices, 24);
}

TEST_F(TextLayoutEngineTest, LayOutTextWrapWhenMaxNumberGlyphsExceeded)
{
    auto layout = layoutEngine_.LayOutText(" !!!!\"\"", fontMetrics_, 4, 1000);

    // Expect:
    //
    // ' !!!!'
    // '""'

    EXPECT_EQ(spaceAdvance_, layout.FixedGlyphWidth);
    EXPECT_EQ(fontGlyphHeight_, layout.GlyphHeight);
    EXPECT_EQ(fontMetrics_.LinespacePixels, layout.LinespacePixels);
    EXPECT_EQ(1000, layout.MaxLineWidthPixels);

    EXPECT_EQ(static_cast<size_t>(2), layout.NumLines());

    // Number of chars in each line

    EXPECT_EQ(5, layout.GetNumCharsInLine(0));
    EXPECT_EQ(2, layout.GetNumCharsInLine(1));

    // Vertices

    auto lineOneVertices = layout.GetVerticesForLine(0);
    EXPECT_EQ(static_cast<size_t>(24 * 4), lineOneVertices.size());
    ExpectGlyphVertices(exclamationCoords_,
        spaceAdvance_ + exclamationAdvance_ * 0, 0, lineOneVertices, 24 * 0);
    ExpectGlyphVertices(exclamationCoords_,
        spaceAdvance_ + exclamationAdvance_ * 1, 0, lineOneVertices, 24 * 1);
    ExpectGlyphVertices(exclamationCoords_,
        spaceAdvance_ + exclamationAdvance_ * 2, 0, lineOneVertices, 24 * 2);
    ExpectGlyphVertices(exclamationCoords_,
        spaceAdvance_ + exclamationAdvance_ * 3, 0, lineOneVertices, 24 * 3);

    auto lineTwoVertices = layout.GetVerticesForLine(1);
    EXPECT_EQ(static_cast<size_t>(24 * 2), lineTwoVertices.size());
    ExpectGlyphVertices(quoteCoords_, quoteAdvance_ * 0,
        fontMetrics_.LinespacePixels, lineTwoVertices, 24 * 0);
    ExpectGlyphVertices(quoteCoords_, quoteAdvance_ * 1,
        fontMetrics_.LinespacePixels, lineTwoVertices, 24 * 1);
}

TEST_F(TextLayoutEngineTest, LayOutTextWrapWhenMaxPixelWidthExceeded)
{
    auto layout = layoutEngine_.LayOutText(
        "    ! !!!!!!", fontMetrics_, 1000, spaceAdvance_ * 3);

    // Expect:
    //
    // '   '
    // ' ! '
    // '!!!!!'
    // '!'

    EXPECT_EQ(spaceAdvance_, layout.FixedGlyphWidth);
    EXPECT_EQ(fontGlyphHeight_, layout.GlyphHeight);
    EXPECT_EQ(fontMetrics_.LinespacePixels, layout.LinespacePixels);
    EXPECT_EQ(spaceAdvance_ * 3, layout.MaxLineWidthPixels);

    EXPECT_EQ(static_cast<size_t>(4), layout.NumLines());

    // Number of chars in each line

    EXPECT_EQ(3, layout.GetNumCharsInLine(0));
    EXPECT_EQ(3, layout.GetNumCharsInLine(1));
    EXPECT_EQ(5, layout.GetNumCharsInLine(2));
    EXPECT_EQ(1, layout.GetNumCharsInLine(3));

    // Vertices

    EXPECT_EQ(static_cast<size_t>(0), layout.GetVerticesForLine(0).size());

    auto lineTwoVertices = layout.GetVerticesForLine(1);
    EXPECT_EQ(static_cast<size_t>(24), lineTwoVertices.size());
    ExpectGlyphVertices(exclamationCoords_, spaceAdvance_,
        fontMetrics_.LinespacePixels, lineTwoVertices, 0);

    auto lineThreeVertices = layout.GetVerticesForLine(2);
    EXPECT_EQ(static_cast<size_t>(24 * 5), lineThreeVertices.size());
    ExpectGlyphVertices(exclamationCoords_, exclamationAdvance_ * 0,
        fontMetrics_.LinespacePixels * 2, lineThreeVertices, 24 * 0);
    ExpectGlyphVertices(exclamationCoords_, exclamationAdvance_ * 1,
        fontMetrics_.LinespacePixels * 2, lineThreeVertices, 24 * 1);
    ExpectGlyphVertices(exclamationCoords_, exclamationAdvance_ * 2,
        fontMetrics_.LinespacePixels * 2, lineThreeVertices, 24 * 2);
    ExpectGlyphVertices(exclamationCoords_, exclamationAdvance_ * 3,
        fontMetrics_.LinespacePixels * 2, lineThreeVertices, 24 * 3);
    ExpectGlyphVertices(exclamationCoords_, exclamationAdvance_ * 4,
        fontMetrics_.LinespacePixels * 2, lineThreeVertices, 24 * 4);

    auto lineFourVertices = layout.GetVerticesForLine(3);
    EXPECT_EQ(static_cast<size_t>(24), lineFourVertices.size());
    ExpectGlyphVertices(exclamationCoords_, 0, fontMetrics_.LinespacePixels * 3,
        lineFourVertices, 0);
}

TEST_F(TextLayoutEngineTest, LayOutTextEmbeddedNewline)
{
    auto layout = layoutEngine_.LayOutText("!\n !!", fontMetrics_, 100, 1000);

    // Expect:
    //
    // '!'
    // ' !!'

    EXPECT_EQ(spaceAdvance_, layout.FixedGlyphWidth);
    EXPECT_EQ(fontGlyphHeight_, layout.GlyphHeight);
    EXPECT_EQ(fontMetrics_.LinespacePixels, layout.LinespacePixels);
    EXPECT_EQ(1000, layout.MaxLineWidthPixels);

    EXPECT_EQ(static_cast<size_t>(2), layout.NumLines());

    // Number of chars in each line

    EXPECT_EQ(1, layout.GetNumCharsInLine(0));
    EXPECT_EQ(3, layout.GetNumCharsInLine(1));

    // Vertices

    auto lineOneVertices = layout.GetVerticesForLine(0);
    EXPECT_EQ(static_cast<size_t>(24), lineOneVertices.size());
    ExpectGlyphVertices(exclamationCoords_, 0, 0, lineOneVertices, 0);

    auto lineTwoVertices = layout.GetVerticesForLine(1);
    EXPECT_EQ(static_cast<size_t>(24 * 2), lineTwoVertices.size());
    ExpectGlyphVertices(exclamationCoords_, spaceAdvance_,
        fontMetrics_.LinespacePixels, lineTwoVertices, 24 * 0);
    ExpectGlyphVertices(exclamationCoords_, spaceAdvance_ + exclamationAdvance_,
        fontMetrics_.LinespacePixels, lineTwoVertices, 24 * 1);
}
