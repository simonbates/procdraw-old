#include "procdraw/utils/font_utils.h"
#include "gtest/gtest.h"

class TextLayoutTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        layout_.FixedGlyphWidth = 10;
        layout_.GlyphHeight = 14;
        layout_.LinespacePixels = 16;
        layout_.MaxLineWidthPixels = 100;
    }

    procdraw::TextLayout<float> layout_;
};

TEST_F(TextLayoutTest, BlockCursorAtOriginForEmptyLayout)
{
    int x, y, width, height;
    layout_.CalculateFixedWidthBlockCursorPos(0, x, y, width, height);
    EXPECT_EQ(0, x);
    EXPECT_EQ(0, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}

TEST_F(TextLayoutTest, BlockCursorAtOriginForSingleEmptyLine)
{
    int x, y, width, height;
    layout_.OpenNewLine();
    layout_.CalculateFixedWidthBlockCursorPos(0, x, y, width, height);
    EXPECT_EQ(0, x);
    EXPECT_EQ(0, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}

TEST_F(TextLayoutTest, BlockCursorAtEndOfSingleLine)
{
    int x, y, width, height;
    layout_.OpenNewLine();
    layout_.SetNumCharsInLine(8);
    layout_.CalculateFixedWidthBlockCursorPos(8, x, y, width, height);
    EXPECT_EQ(80, x);
    EXPECT_EQ(0, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}

TEST_F(TextLayoutTest, BlockCursorWrapsWhenExceedsMaxLineWidthPixels)
{
    int x, y, width, height;
    layout_.MaxLineWidthPixels = 89;
    layout_.OpenNewLine();
    layout_.SetNumCharsInLine(8);
    layout_.CalculateFixedWidthBlockCursorPos(8, x, y, width, height);
    EXPECT_EQ(0, x);
    EXPECT_EQ(16, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}

TEST_F(TextLayoutTest, BlockCursorNoWrapWhenRightAtMaxLineWidthPixels)
{
    int x, y, width, height;
    layout_.MaxLineWidthPixels = 90;
    layout_.OpenNewLine();
    layout_.SetNumCharsInLine(8);
    layout_.CalculateFixedWidthBlockCursorPos(8, x, y, width, height);
    EXPECT_EQ(80, x);
    EXPECT_EQ(0, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}

TEST_F(TextLayoutTest, BlockCursorOnSecondLine)
{
    int x, y, width, height;
    layout_.OpenNewLine();
    layout_.SetNumCharsInLine(8);
    layout_.OpenNewLine();
    layout_.SetNumCharsInLine(4);
    layout_.CalculateFixedWidthBlockCursorPos(10, x, y, width, height);
    EXPECT_EQ(20, x);
    EXPECT_EQ(16, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}

TEST_F(TextLayoutTest, BlockCursorBeyondCharCount)
{
    int x, y, width, height;
    layout_.OpenNewLine();
    layout_.SetNumCharsInLine(8);
    layout_.CalculateFixedWidthBlockCursorPos(100, x, y, width, height);
    EXPECT_EQ(0, x);
    EXPECT_EQ(32, y);
    EXPECT_EQ(10, width);
    EXPECT_EQ(14, height);
}
