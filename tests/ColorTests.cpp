/* Copyright (c) 2014, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Color.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

// clang-format off

TEST_CLASS(ColorTests) {
public:

    TEST_METHOD(Hsv2rgbRed)
    {
        float r, g, b;
        std::tie(r, g, b) = Hsv2rgb(0.0f, 1.0f, 1.0f);
        Assert::AreEqual(1.0f, r);
        Assert::AreEqual(0.0f, g);
        Assert::AreEqual(0.0f, b);
    }

    TEST_METHOD(Hsv2rgbGreen)
    {
        float r, g, b;
        std::tie(r, g, b) = Hsv2rgb(120.0f, 1.0f, 1.0f);
        Assert::AreEqual(0.0f, r);
        Assert::AreEqual(1.0f, g);
        Assert::AreEqual(0.0f, b);
    }

    TEST_METHOD(Hsv2rgbBblue)
    {
        float r, g, b;
        std::tie(r, g, b) = Hsv2rgb(240.0f, 1.0f, 1.0f);
        Assert::AreEqual(0.0f, r);
        Assert::AreEqual(0.0f, g);
        Assert::AreEqual(1.0f, b);
    }

};

}
}
