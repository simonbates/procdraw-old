/* Copyright (c) 2015, 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "ProcdrawMath.h"

namespace Procdraw {
namespace Tests {

TEST(ProcdrawMathTest, ClampInt)
{
    EXPECT_EQ(100, Clamp(50, 100, 200));
    EXPECT_EQ(150, Clamp(150, 100, 200));
    EXPECT_EQ(200, Clamp(250, 100, 200));
}

TEST(ProcdrawMathTest, ClampFloat)
{
    EXPECT_EQ(100.0f, Clamp(50.0f, 100.0f, 200.0f));
    EXPECT_EQ(150.0f, Clamp(150.0f, 100.0f, 200.0f));
    EXPECT_EQ(200.0f, Clamp(250.0f, 100.0f, 200.0f));
}

TEST(ProcdrawMathTest, ApproximatelyEqualTest)
{
    EXPECT_FALSE(ApproximatelyEqual(1.0, 1.05, 0.001));
    EXPECT_FALSE(ApproximatelyEqual(1.0, 1.05, 0.01));
    EXPECT_TRUE(ApproximatelyEqual(1.0, 1.05, 0.1));
}

TEST(ProcdrawMathTest, LerpTest)
{
    // [0, 8]

    EXPECT_EQ(0.0, Lerp(0.0, 8.0, 0.0));
    EXPECT_EQ(2.0, Lerp(0.0, 8.0, 0.25));
    EXPECT_EQ(4.0, Lerp(0.0, 8.0, 0.5));
    EXPECT_EQ(6.0, Lerp(0.0, 8.0, 0.75));
    EXPECT_EQ(8.0, Lerp(0.0, 8.0, 1.0));

    // [4, -4]

    EXPECT_EQ(4.0, Lerp(4.0, -4.0, 0.0));
    EXPECT_EQ(2.0, Lerp(4.0, -4.0, 0.25));
    EXPECT_EQ(0.0, Lerp(4.0, -4.0, 0.5));
    EXPECT_EQ(-2.0, Lerp(4.0, -4.0, 0.75));
    EXPECT_EQ(-4.0, Lerp(4.0, -4.0, 1.0));
}

TEST(ProcdrawMathTest, MapRangeTest)
{
    // [0, 10] to [-1, 0]

    EXPECT_EQ(-1.0, MapRange(0.0, 10.0, -1.0, 0.0, 0.0));
    EXPECT_EQ(-0.75, MapRange(0.0, 10.0, -1.0, 0.0, 2.5));
    EXPECT_EQ(-0.5, MapRange(0.0, 10.0, -1.0, 0.0, 5.0));
    EXPECT_EQ(-0.25, MapRange(0.0, 10.0, -1.0, 0.0, 7.5));
    EXPECT_EQ(0.0, MapRange(0.0, 10.0, -1.0, 0.0, 10.0));

    // [0, 10] to [1, -1]

    EXPECT_EQ(1.0, MapRange(0.0, 10.0, 1.0, -1.0, 0.0));
    EXPECT_EQ(0.5, MapRange(0.0, 10.0, 1.0, -1.0, 2.5));
    EXPECT_EQ(0.0, MapRange(0.0, 10.0, 1.0, -1.0, 5.0));
    EXPECT_EQ(-0.5, MapRange(0.0, 10.0, 1.0, -1.0, 7.5));
    EXPECT_EQ(-1.0, MapRange(0.0, 10.0, 1.0, -1.0, 10.0));
}

TEST(ProcdrawMathTest, NormTest)
{
    // [0, 8]

    EXPECT_EQ(0.0, Norm(0.0, 8.0, 0.0));
    EXPECT_EQ(0.25, Norm(0.0, 8.0, 2.0));
    EXPECT_EQ(0.5, Norm(0.0, 8.0, 4.0));
    EXPECT_EQ(0.75, Norm(0.0, 8.0, 6.0));
    EXPECT_EQ(1.0, Norm(0.0, 8.0, 8.0));

    // [4, -4]

    EXPECT_EQ(0.0, Norm(4.0, -4.0, 4.0));
    EXPECT_EQ(0.25, Norm(4.0, -4.0, 2.0));
    EXPECT_EQ(0.5, Norm(4.0, -4.0, 0.0));
    EXPECT_EQ(0.75, Norm(4.0, -4.0, -2.0));
    EXPECT_EQ(1.0, Norm(4.0, -4.0, -4.0));
}

TEST(ProcdrawMathTest, PowerOf2GteTest)
{
    EXPECT_EQ(0, PowerOf2Gte(0));
    EXPECT_EQ(0, PowerOf2Gte(-1));
    EXPECT_EQ(0, PowerOf2Gte(-100));
    EXPECT_EQ(1, PowerOf2Gte(1));
    EXPECT_EQ(2, PowerOf2Gte(2));
    EXPECT_EQ(4, PowerOf2Gte(3));
    EXPECT_EQ(4, PowerOf2Gte(4));
    EXPECT_EQ(8, PowerOf2Gte(5));
    EXPECT_EQ(128, PowerOf2Gte(127));
    EXPECT_EQ(128, PowerOf2Gte(128));
}

TEST(ProcdrawMathTest, WrapTest)
{
    // [0, 10]

    EXPECT_EQ(0.0, Wrap(0.0, 10.0, 0.0));
    EXPECT_EQ(0.0, Wrap(0.0, 10.0, 10.0));
    EXPECT_EQ(8.0, Wrap(0.0, 10.0, 8.0));
    EXPECT_EQ(2.0, Wrap(0.0, 10.0, 12.0));
    EXPECT_EQ(0.0, Wrap(0.0, 10.0, 20.0));
    EXPECT_EQ(3.0, Wrap(0.0, 10.0, 23.0));
    EXPECT_EQ(8.0, Wrap(0.0, 10.0, -2.0));
    EXPECT_EQ(0.0, Wrap(0.0, 10.0, -10.0));
    EXPECT_EQ(7.0, Wrap(0.0, 10.0, -13.0));

    // [-20, -10]

    EXPECT_EQ(-20.0, Wrap(-20.0, -10.0, -20.0));
    EXPECT_EQ(-20.0, Wrap(-20.0, -10.0, -10.0));
    EXPECT_EQ(-12.0, Wrap(-20.0, -10.0, -12.0));
    EXPECT_EQ(-18.0, Wrap(-20.0, -10.0, -8.0));
    EXPECT_EQ(-20.0, Wrap(-20.0, -10.0, 0.0));
    EXPECT_EQ(-17.0, Wrap(-20.0, -10.0, 13.0));
    EXPECT_EQ(-12.0, Wrap(-20.0, -10.0, -22.0));
    EXPECT_EQ(-20.0, Wrap(-20.0, -10.0, -30.0));
    EXPECT_EQ(-13.0, Wrap(-20.0, -10.0, -33.0));
}

} // namespace Tests
} // namespace Procdraw
