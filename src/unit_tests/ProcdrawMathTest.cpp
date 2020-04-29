// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "../lib/ProcdrawMath.h"
#include <gtest/gtest.h>

using namespace Procdraw;

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

TEST(ProcdrawMathTest, ApproximatelyEqual)
{
    EXPECT_FALSE(ApproximatelyEqual(1.0, 1.05, 0.001));
    EXPECT_FALSE(ApproximatelyEqual(1.0, 1.05, 0.01));
    EXPECT_TRUE(ApproximatelyEqual(1.0, 1.05, 0.1));
}

TEST(ProcdrawMathTest, Lerp)
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

TEST(ProcdrawMathTest, MapRange)
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

TEST(ProcdrawMathTest, Norm)
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

TEST(ProcdrawMathTest, PowerOf2Gte)
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

TEST(ProcdrawMathTest, Wrap)
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
