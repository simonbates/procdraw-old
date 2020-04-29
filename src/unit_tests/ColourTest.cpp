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

#include "../lib/Colour.h"
#include <gtest/gtest.h>

using namespace Procdraw;

TEST(ColourTest, Hsv2rgbRed)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(0.0f, 1.0f, 1.0f);
    EXPECT_EQ(1.0f, r);
    EXPECT_EQ(0.0f, g);
    EXPECT_EQ(0.0f, b);
}

TEST(ColourTest, Hsv2rgbGreen)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(120.0f, 1.0f, 1.0f);
    EXPECT_EQ(0.0f, r);
    EXPECT_EQ(1.0f, g);
    EXPECT_EQ(0.0f, b);
}

TEST(ColourTest, Hsv2rgbBlue)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(240.0f, 1.0f, 1.0f);
    EXPECT_EQ(0.0f, r);
    EXPECT_EQ(0.0f, g);
    EXPECT_EQ(1.0f, b);
}
