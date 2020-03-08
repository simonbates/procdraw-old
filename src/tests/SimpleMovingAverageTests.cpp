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
#include <catch.hpp>

using namespace Procdraw;

TEST_CASE("SimpleMovingAverage")
{
    SimpleMovingAverage<int> sma(4);

    REQUIRE(sma.GetMean() == 0);

    sma.AddDataPoint(8);
    // 0, 0, 0, 8
    REQUIRE(sma.GetMean() == 2);

    sma.AddDataPoint(16);
    // 0, 0, 8, 16
    REQUIRE(sma.GetMean() == 6);

    sma.AddDataPoint(32);
    // 0, 8, 16, 32
    REQUIRE(sma.GetMean() == 14);

    sma.AddDataPoint(64);
    // 8, 16, 32, 64
    REQUIRE(sma.GetMean() == 30);

    sma.AddDataPoint(128);
    // 16, 32, 64, 128
    REQUIRE(sma.GetMean() == 60);

    sma.AddDataPoint(0);
    // 32, 64, 128, 0
    REQUIRE(sma.GetMean() == 56);
}
