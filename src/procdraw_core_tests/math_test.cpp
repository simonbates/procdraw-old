#include "procdraw_core/math.h"
#include "gtest/gtest.h"

TEST(MathTest, ClampInt)
{
  EXPECT_EQ(100, procdraw::Clamp(50, 100, 200));
  EXPECT_EQ(150, procdraw::Clamp(150, 100, 200));
  EXPECT_EQ(200, procdraw::Clamp(250, 100, 200));
}

TEST(MathTest, ClampFloat)
{
  EXPECT_EQ(100.0f, procdraw::Clamp(50.0f, 100.0f, 200.0f));
  EXPECT_EQ(150.0f, procdraw::Clamp(150.0f, 100.0f, 200.0f));
  EXPECT_EQ(200.0f, procdraw::Clamp(250.0f, 100.0f, 200.0f));
}

TEST(MathTest, ApproximatelyEqual)
{
  EXPECT_FALSE(procdraw::ApproximatelyEqual(1, 1.05, 0.001));
  EXPECT_FALSE(procdraw::ApproximatelyEqual(1, 1.05, 0.01));
  EXPECT_TRUE(procdraw::ApproximatelyEqual(1, 1.05, 0.1));
}

TEST(MathTest, Lerp)
{
  // [0, 8]

  EXPECT_EQ(0.0, procdraw::Lerp(0.0, 8.0, 0.0));
  EXPECT_EQ(2.0, procdraw::Lerp(0.0, 8.0, 0.25));
  EXPECT_EQ(4.0, procdraw::Lerp(0.0, 8.0, 0.5));
  EXPECT_EQ(6.0, procdraw::Lerp(0.0, 8.0, 0.75));
  EXPECT_EQ(8.0, procdraw::Lerp(0.0, 8.0, 1.0));

  // [4, -4]

  EXPECT_EQ(4.0, procdraw::Lerp(4.0, -4.0, 0.0));
  EXPECT_EQ(2.0, procdraw::Lerp(4.0, -4.0, 0.25));
  EXPECT_EQ(0.0, procdraw::Lerp(4.0, -4.0, 0.5));
  EXPECT_EQ(-2.0, procdraw::Lerp(4.0, -4.0, 0.75));
  EXPECT_EQ(-4.0, procdraw::Lerp(4.0, -4.0, 1.0));
}

TEST(MathTest, MapRange)
{
  // [0, 10] to [-1, 0]

  EXPECT_EQ(-1.0, procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 0.0));
  EXPECT_EQ(-0.75, procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 2.5));
  EXPECT_EQ(-0.5, procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 5.0));
  EXPECT_EQ(-0.25, procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 7.5));
  EXPECT_EQ(0.0, procdraw::MapRange(0.0, 10.0, -1.0, 0.0, 10.0));

  // [0, 10] to [1, -1]

  EXPECT_EQ(1.0, procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 0.0));
  EXPECT_EQ(0.5, procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 2.5));
  EXPECT_EQ(0.0, procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 5.0));
  EXPECT_EQ(-0.5, procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 7.5));
  EXPECT_EQ(-1.0, procdraw::MapRange(0.0, 10.0, 1.0, -1.0, 10.0));
}

TEST(MathTest, Norm)
{
  // [0, 8]

  EXPECT_EQ(0.0, procdraw::Norm(0.0, 8.0, 0.0));
  EXPECT_EQ(0.25, procdraw::Norm(0.0, 8.0, 2.0));
  EXPECT_EQ(0.5, procdraw::Norm(0.0, 8.0, 4.0));
  EXPECT_EQ(0.75, procdraw::Norm(0.0, 8.0, 6.0));
  EXPECT_EQ(1.0, procdraw::Norm(0.0, 8.0, 8.0));

  // [4, -4]

  EXPECT_EQ(0.0, procdraw::Norm(4.0, -4.0, 4.0));
  EXPECT_EQ(0.25, procdraw::Norm(4.0, -4.0, 2.0));
  EXPECT_EQ(0.5, procdraw::Norm(4.0, -4.0, 0.0));
  EXPECT_EQ(0.75, procdraw::Norm(4.0, -4.0, -2.0));
  EXPECT_EQ(1.0, procdraw::Norm(4.0, -4.0, -4.0));
}

TEST(MathTest, PowerOf2Gte)
{
  EXPECT_EQ(0, procdraw::PowerOf2Gte(0));
  EXPECT_EQ(0, procdraw::PowerOf2Gte(-1));
  EXPECT_EQ(0, procdraw::PowerOf2Gte(-100));
  EXPECT_EQ(1, procdraw::PowerOf2Gte(1));
  EXPECT_EQ(2, procdraw::PowerOf2Gte(2));
  EXPECT_EQ(4, procdraw::PowerOf2Gte(3));
  EXPECT_EQ(4, procdraw::PowerOf2Gte(4));
  EXPECT_EQ(8, procdraw::PowerOf2Gte(5));
  EXPECT_EQ(128, procdraw::PowerOf2Gte(127));
  EXPECT_EQ(128, procdraw::PowerOf2Gte(128));
}

TEST(MathTest, Wrap)
{
  // [0, 10]

  EXPECT_EQ(0.0, procdraw::Wrap(0.0, 10.0, 0.0));
  EXPECT_EQ(0.0, procdraw::Wrap(0.0, 10.0, 10.0));
  EXPECT_EQ(0.0, procdraw::Wrap(0.0, 10.0, 20.0));
  EXPECT_EQ(0.0, procdraw::Wrap(0.0, 10.0, -10.0));
  EXPECT_EQ(8.0, procdraw::Wrap(0.0, 10.0, 8.0));
  EXPECT_EQ(2.0, procdraw::Wrap(0.0, 10.0, 12.0));
  EXPECT_EQ(3.0, procdraw::Wrap(0.0, 10.0, 23.0));
  EXPECT_EQ(8.0, procdraw::Wrap(0.0, 10.0, -2.0));
  EXPECT_EQ(7.0, procdraw::Wrap(0.0, 10.0, -13.0));

  // [-20, -10]

  EXPECT_EQ(-20.0, procdraw::Wrap(-20.0, -10.0, -20.0));
  EXPECT_EQ(-20.0, procdraw::Wrap(-20.0, -10.0, -10.0));
  EXPECT_EQ(-20.0, procdraw::Wrap(-20.0, -10.0, 0.0));
  EXPECT_EQ(-20.0, procdraw::Wrap(-20.0, -10.0, -30.0));
  EXPECT_EQ(-12.0, procdraw::Wrap(-20.0, -10.0, -12.0));
  EXPECT_EQ(-18.0, procdraw::Wrap(-20.0, -10.0, -8.0));
  EXPECT_EQ(-17.0, procdraw::Wrap(-20.0, -10.0, 13.0));
  EXPECT_EQ(-12.0, procdraw::Wrap(-20.0, -10.0, -22.0));
  EXPECT_EQ(-13.0, procdraw::Wrap(-20.0, -10.0, -33.0));
}

TEST(MathTest, SimpleMovingAverage)
{
  procdraw::SimpleMovingAverage<int> sma(4);

  EXPECT_EQ(0, sma.GetMean());

  sma.AddDataPoint(8);
  // 0, 0, 0, 8
  EXPECT_EQ(2, sma.GetMean());

  sma.AddDataPoint(16);
  // 0, 0, 8, 16
  EXPECT_EQ(6, sma.GetMean());

  sma.AddDataPoint(32);
  // 0, 8, 16, 32
  EXPECT_EQ(14, sma.GetMean());

  sma.AddDataPoint(64);
  // 8, 16, 32, 64
  EXPECT_EQ(30, sma.GetMean());

  sma.AddDataPoint(128);
  // 16, 32, 64, 128
  EXPECT_EQ(60, sma.GetMean());

  sma.AddDataPoint(0);
  // 32, 64, 128, 0
  EXPECT_EQ(56, sma.GetMean());
}
