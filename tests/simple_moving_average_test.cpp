#include "procdraw/simple_moving_average.h"
#include "gtest/gtest.h"

TEST(SimpleMovingAverageTest, SimpleMovingAverage)
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
