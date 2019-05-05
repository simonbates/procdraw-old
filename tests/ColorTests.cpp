/* Copyright (c) 2014, 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Color.h"
#include <gtest/gtest.h>

namespace Procdraw {
namespace Tests {

// clang-format off

TEST(ColorTests, Hsv2rgbRed)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(0.0f, 1.0f, 1.0f);
    ASSERT_EQ(1.0f, r);
    ASSERT_EQ(0.0f, g);
    ASSERT_EQ(0.0f, b);
}

TEST(ColorTests, Hsv2rgbGreen)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(120.0f, 1.0f, 1.0f);
    ASSERT_EQ(0.0f, r);
    ASSERT_EQ(1.0f, g);
    ASSERT_EQ(0.0f, b);
}

TEST(ColorTests, Hsv2rgbBblue)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(240.0f, 1.0f, 1.0f);
    ASSERT_EQ(0.0f, r);
    ASSERT_EQ(0.0f, g);
    ASSERT_EQ(1.0f, b);
}

}
}
