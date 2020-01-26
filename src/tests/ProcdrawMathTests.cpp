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
#include <catch.hpp>

using namespace Procdraw;

TEST_CASE("Clamp int")
{
    REQUIRE(Clamp(50, 100, 200) == 100);
    REQUIRE(Clamp(150, 100, 200) == 150);
    REQUIRE(Clamp(250, 100, 200) == 200);
}

TEST_CASE("Clamp float")
{
    REQUIRE(Clamp(50.0f, 100.0f, 200.0f) == 100.0f);
    REQUIRE(Clamp(150.0f, 100.0f, 200.0f) == 150.0f);
    REQUIRE(Clamp(250.0f, 100.0f, 200.0f) == 200.0f);
}

TEST_CASE("ApproximatelyEqual")
{
    REQUIRE_FALSE(ApproximatelyEqual(1.0, 1.05, 0.001));
    REQUIRE_FALSE(ApproximatelyEqual(1.0, 1.05, 0.01));
    REQUIRE(ApproximatelyEqual(1.0, 1.05, 0.1));
}

TEST_CASE("Lerp")
{
    // [0, 8]

    REQUIRE(Lerp(0.0, 8.0, 0.0) == 0.0);
    REQUIRE(Lerp(0.0, 8.0, 0.25) == 2.0);
    REQUIRE(Lerp(0.0, 8.0, 0.5) == 4.0);
    REQUIRE(Lerp(0.0, 8.0, 0.75) == 6.0);
    REQUIRE(Lerp(0.0, 8.0, 1.0) == 8.0);

    // [4, -4]

    REQUIRE(Lerp(4.0, -4.0, 0.0) == 4.0);
    REQUIRE(Lerp(4.0, -4.0, 0.25) == 2.0);
    REQUIRE(Lerp(4.0, -4.0, 0.5) == 0.0);
    REQUIRE(Lerp(4.0, -4.0, 0.75) == -2.0);
    REQUIRE(Lerp(4.0, -4.0, 1.0) == -4.0);
}

TEST_CASE("MapRange")
{
    // [0, 10] to [-1, 0]

    REQUIRE(MapRange(0.0, 10.0, -1.0, 0.0, 0.0) == -1.0);
    REQUIRE(MapRange(0.0, 10.0, -1.0, 0.0, 2.5) == -0.75);
    REQUIRE(MapRange(0.0, 10.0, -1.0, 0.0, 5.0) == -0.5);
    REQUIRE(MapRange(0.0, 10.0, -1.0, 0.0, 7.5) == -0.25);
    REQUIRE(MapRange(0.0, 10.0, -1.0, 0.0, 10.0) == 0.0);

    // [0, 10] to [1, -1]

    REQUIRE(MapRange(0.0, 10.0, 1.0, -1.0, 0.0) == 1.0);
    REQUIRE(MapRange(0.0, 10.0, 1.0, -1.0, 2.5) == 0.5);
    REQUIRE(MapRange(0.0, 10.0, 1.0, -1.0, 5.0) == 0.0);
    REQUIRE(MapRange(0.0, 10.0, 1.0, -1.0, 7.5) == -0.5);
    REQUIRE(MapRange(0.0, 10.0, 1.0, -1.0, 10.0) == -1.0);
}

TEST_CASE("Norm")
{
    // [0, 8]

    REQUIRE(Norm(0.0, 8.0, 0.0) == 0.0);
    REQUIRE(Norm(0.0, 8.0, 2.0) == 0.25);
    REQUIRE(Norm(0.0, 8.0, 4.0) == 0.5);
    REQUIRE(Norm(0.0, 8.0, 6.0) == 0.75);
    REQUIRE(Norm(0.0, 8.0, 8.0) == 1.0);

    // [4, -4]

    REQUIRE(Norm(4.0, -4.0, 4.0) == 0.0);
    REQUIRE(Norm(4.0, -4.0, 2.0) == 0.25);
    REQUIRE(Norm(4.0, -4.0, 0.0) == 0.5);
    REQUIRE(Norm(4.0, -4.0, -2.0) == 0.75);
    REQUIRE(Norm(4.0, -4.0, -4.0) == 1.0);
}

TEST_CASE("PowerOf2Gte")
{
    REQUIRE(PowerOf2Gte(0) == 0);
    REQUIRE(PowerOf2Gte(-1) == 0);
    REQUIRE(PowerOf2Gte(-100) == 0);
    REQUIRE(PowerOf2Gte(1) == 1);
    REQUIRE(PowerOf2Gte(2) == 2);
    REQUIRE(PowerOf2Gte(3) == 4);
    REQUIRE(PowerOf2Gte(4) == 4);
    REQUIRE(PowerOf2Gte(5) == 8);
    REQUIRE(PowerOf2Gte(127) == 128);
    REQUIRE(PowerOf2Gte(128) == 128);
}

TEST_CASE("Wrap")
{
    // [0, 10]

    REQUIRE(Wrap(0.0, 10.0, 0.0) == 0.0);
    REQUIRE(Wrap(0.0, 10.0, 10.0) == 0.0);
    REQUIRE(Wrap(0.0, 10.0, 8.0) == 8.0);
    REQUIRE(Wrap(0.0, 10.0, 12.0) == 2.0);
    REQUIRE(Wrap(0.0, 10.0, 20.0) == 0.0);
    REQUIRE(Wrap(0.0, 10.0, 23.0) == 3.0);
    REQUIRE(Wrap(0.0, 10.0, -2.0) == 8.0);
    REQUIRE(Wrap(0.0, 10.0, -10.0) == 0.0);
    REQUIRE(Wrap(0.0, 10.0, -13.0) == 7.0);

    // [-20, -10]

    REQUIRE(Wrap(-20.0, -10.0, -20.0) == -20.0);
    REQUIRE(Wrap(-20.0, -10.0, -10.0) == -20.0);
    REQUIRE(Wrap(-20.0, -10.0, -12.0) == -12.0);
    REQUIRE(Wrap(-20.0, -10.0, -8.0) == -18.0);
    REQUIRE(Wrap(-20.0, -10.0, 0.0) == -20.0);
    REQUIRE(Wrap(-20.0, -10.0, 13.0) == -17.0);
    REQUIRE(Wrap(-20.0, -10.0, -22.0) == -12.0);
    REQUIRE(Wrap(-20.0, -10.0, -30.0) == -20.0);
    REQUIRE(Wrap(-20.0, -10.0, -33.0) == -13.0);
}
