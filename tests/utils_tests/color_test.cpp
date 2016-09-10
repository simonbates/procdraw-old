#include "procdraw/utils/color.h"
#include "gtest/gtest.h"

TEST(ColorTest, Hsv2RgbRed)
{
  float r, g, b;
  procdraw::Hsv2Rgb(0.0f, 1.0f, 1.0f, r, g, b);
  EXPECT_EQ(1.0f, r);
  EXPECT_EQ(0.0f, g);
  EXPECT_EQ(0.0f, b);
}

TEST(ColorTest, Hsv2RgbGreen)
{
  float r, g, b;
  procdraw::Hsv2Rgb(120.0f, 1.0f, 1.0f, r, g, b);
  EXPECT_EQ(0.0f, r);
  EXPECT_EQ(1.0f, g);
  EXPECT_EQ(0.0f, b);
}

TEST(ColorTest, Hsv2RgbBlue)
{
  float r, g, b;
  procdraw::Hsv2Rgb(240.0f, 1.0f, 1.0f, r, g, b);
  EXPECT_EQ(0.0f, r);
  EXPECT_EQ(0.0f, g);
  EXPECT_EQ(1.0f, b);
}
