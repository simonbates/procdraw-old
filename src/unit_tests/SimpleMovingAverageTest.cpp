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

#include "../lib/SimpleMovingAverage.h"
#include <gtest/gtest.h>

using namespace Procdraw;

TEST(SimpleMovingAverageTest, SimpleMovingAverage)
{
    SimpleMovingAverage<int> sma(4);

    ASSERT_EQ(0, sma.GetMean());

    sma.AddDataPoint(8);
    // 0, 0, 0, 8
    ASSERT_EQ(2, sma.GetMean());

    sma.AddDataPoint(16);
    // 0, 0, 8, 16
    ASSERT_EQ(6, sma.GetMean());

    sma.AddDataPoint(32);
    // 0, 8, 16, 32
    ASSERT_EQ(14, sma.GetMean());

    sma.AddDataPoint(64);
    // 8, 16, 32, 64
    ASSERT_EQ(30, sma.GetMean());

    sma.AddDataPoint(128);
    // 16, 32, 64, 128
    ASSERT_EQ(60, sma.GetMean());

    sma.AddDataPoint(0);
    // 32, 64, 128, 0
    ASSERT_EQ(56, sma.GetMean());
}
