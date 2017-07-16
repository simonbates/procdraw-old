#include "procdraw/color.h"
#include <gtest/gtest.h>

TEST(ColorTest, Hsv2rgbRed)
{
    float r, g, b;
    std::tie(r, g, b) = procdraw::hsv2rgb(0.0f, 1.0f, 1.0f);
    EXPECT_EQ(1.0f, r);
    EXPECT_EQ(0.0f, g);
    EXPECT_EQ(0.0f, b);
}

TEST(ColorTest, Hsv2rgbGreen)
{
    float r, g, b;
    std::tie(r, g, b) = procdraw::hsv2rgb(120.0f, 1.0f, 1.0f);
    EXPECT_EQ(0.0f, r);
    EXPECT_EQ(1.0f, g);
    EXPECT_EQ(0.0f, b);
}

TEST(ColorTest, Hsv2rgbBlue)
{
    float r, g, b;
    std::tie(r, g, b) = procdraw::hsv2rgb(240.0f, 1.0f, 1.0f);
    EXPECT_EQ(0.0f, r);
    EXPECT_EQ(0.0f, g);
    EXPECT_EQ(1.0f, b);
}
