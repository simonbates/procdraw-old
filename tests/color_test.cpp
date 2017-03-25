#include "procdraw/color.h"
#include "gtest/gtest.h"

TEST(ColorTest, hsv2rgb_red)
{
    float r, g, b;
    std::tie(r, g, b) = procdraw::hsv2rgb(0.0f, 1.0f, 1.0f);
    EXPECT_EQ(1.0f, r);
    EXPECT_EQ(0.0f, g);
    EXPECT_EQ(0.0f, b);
}

TEST(ColorTest, hsv2rgb_green)
{
    float r, g, b;
    std::tie(r, g, b) = procdraw::hsv2rgb(120.0f, 1.0f, 1.0f);
    EXPECT_EQ(0.0f, r);
    EXPECT_EQ(1.0f, g);
    EXPECT_EQ(0.0f, b);
}

TEST(ColorTest, hsv2rgb_blue)
{
    float r, g, b;
    std::tie(r, g, b) = procdraw::hsv2rgb(240.0f, 1.0f, 1.0f);
    EXPECT_EQ(0.0f, r);
    EXPECT_EQ(0.0f, g);
    EXPECT_EQ(1.0f, b);
}
