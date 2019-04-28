/* Copyright (c) 2015, 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "ProcdrawMath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

// clang-format off

TEST_CLASS(ProcdrawMathTests) {
public:

    TEST_METHOD(ClampInt)
    {
        Assert::AreEqual(100, Clamp(50, 100, 200));
        Assert::AreEqual(150, Clamp(150, 100, 200));
        Assert::AreEqual(200, Clamp(250, 100, 200));
    }

    TEST_METHOD(ClampFloat)
    {
        Assert::AreEqual(100.0f, Clamp(50.0f, 100.0f, 200.0f));
        Assert::AreEqual(150.0f, Clamp(150.0f, 100.0f, 200.0f));
        Assert::AreEqual(200.0f, Clamp(250.0f, 100.0f, 200.0f));
    }

    TEST_METHOD(ApproximatelyEqualTest)
    {
        Assert::IsFalse(ApproximatelyEqual(1.0, 1.05, 0.001));
        Assert::IsFalse(ApproximatelyEqual(1.0, 1.05, 0.01));
        Assert::IsTrue(ApproximatelyEqual(1.0, 1.05, 0.1));
    }

    TEST_METHOD(LerpTest)
    {
        // [0, 8]

        Assert::AreEqual(0.0, Lerp(0.0, 8.0, 0.0));
        Assert::AreEqual(2.0, Lerp(0.0, 8.0, 0.25));
        Assert::AreEqual(4.0, Lerp(0.0, 8.0, 0.5));
        Assert::AreEqual(6.0, Lerp(0.0, 8.0, 0.75));
        Assert::AreEqual(8.0, Lerp(0.0, 8.0, 1.0));

        // [4, -4]

        Assert::AreEqual(4.0, Lerp(4.0, -4.0, 0.0));
        Assert::AreEqual(2.0, Lerp(4.0, -4.0, 0.25));
        Assert::AreEqual(0.0, Lerp(4.0, -4.0, 0.5));
        Assert::AreEqual(-2.0, Lerp(4.0, -4.0, 0.75));
        Assert::AreEqual(-4.0, Lerp(4.0, -4.0, 1.0));
    }

    TEST_METHOD(MapRangeTest)
    {
        // [0, 10] to [-1, 0]

        Assert::AreEqual(-1.0, MapRange(0.0, 10.0, -1.0, 0.0, 0.0));
        Assert::AreEqual(-0.75, MapRange(0.0, 10.0, -1.0, 0.0, 2.5));
        Assert::AreEqual(-0.5, MapRange(0.0, 10.0, -1.0, 0.0, 5.0));
        Assert::AreEqual(-0.25, MapRange(0.0, 10.0, -1.0, 0.0, 7.5));
        Assert::AreEqual(0.0, MapRange(0.0, 10.0, -1.0, 0.0, 10.0));

        // [0, 10] to [1, -1]

        Assert::AreEqual(1.0, MapRange(0.0, 10.0, 1.0, -1.0, 0.0));
        Assert::AreEqual(0.5, MapRange(0.0, 10.0, 1.0, -1.0, 2.5));
        Assert::AreEqual(0.0, MapRange(0.0, 10.0, 1.0, -1.0, 5.0));
        Assert::AreEqual(-0.5, MapRange(0.0, 10.0, 1.0, -1.0, 7.5));
        Assert::AreEqual(-1.0, MapRange(0.0, 10.0, 1.0, -1.0, 10.0));
    }

    TEST_METHOD(NormTest)
    {
        // [0, 8]

        Assert::AreEqual(0.0, Norm(0.0, 8.0, 0.0));
        Assert::AreEqual(0.25, Norm(0.0, 8.0, 2.0));
        Assert::AreEqual(0.5, Norm(0.0, 8.0, 4.0));
        Assert::AreEqual(0.75, Norm(0.0, 8.0, 6.0));
        Assert::AreEqual(1.0, Norm(0.0, 8.0, 8.0));

        // [4, -4]

        Assert::AreEqual(0.0, Norm(4.0, -4.0, 4.0));
        Assert::AreEqual(0.25, Norm(4.0, -4.0, 2.0));
        Assert::AreEqual(0.5, Norm(4.0, -4.0, 0.0));
        Assert::AreEqual(0.75, Norm(4.0, -4.0, -2.0));
        Assert::AreEqual(1.0, Norm(4.0, -4.0, -4.0));
    }

    TEST_METHOD(PowerOf2GteTest)
    {
        Assert::AreEqual(0, PowerOf2Gte(0));
        Assert::AreEqual(0, PowerOf2Gte(-1));
        Assert::AreEqual(0, PowerOf2Gte(-100));
        Assert::AreEqual(1, PowerOf2Gte(1));
        Assert::AreEqual(2, PowerOf2Gte(2));
        Assert::AreEqual(4, PowerOf2Gte(3));
        Assert::AreEqual(4, PowerOf2Gte(4));
        Assert::AreEqual(8, PowerOf2Gte(5));
        Assert::AreEqual(128, PowerOf2Gte(127));
        Assert::AreEqual(128, PowerOf2Gte(128));
    }

    TEST_METHOD(WrapTest)
    {
        // [0, 10]

        Assert::AreEqual(0.0, Wrap(0.0, 10.0, 0.0));
        Assert::AreEqual(0.0, Wrap(0.0, 10.0, 10.0));
        Assert::AreEqual(8.0, Wrap(0.0, 10.0, 8.0));
        Assert::AreEqual(2.0, Wrap(0.0, 10.0, 12.0));
        Assert::AreEqual(0.0, Wrap(0.0, 10.0, 20.0));
        Assert::AreEqual(3.0, Wrap(0.0, 10.0, 23.0));
        Assert::AreEqual(8.0, Wrap(0.0, 10.0, -2.0));
        Assert::AreEqual(0.0, Wrap(0.0, 10.0, -10.0));
        Assert::AreEqual(7.0, Wrap(0.0, 10.0, -13.0));

        // [-20, -10]

        Assert::AreEqual(-20.0, Wrap(-20.0, -10.0, -20.0));
        Assert::AreEqual(-20.0, Wrap(-20.0, -10.0, -10.0));
        Assert::AreEqual(-12.0, Wrap(-20.0, -10.0, -12.0));
        Assert::AreEqual(-18.0, Wrap(-20.0, -10.0, -8.0));
        Assert::AreEqual(-20.0, Wrap(-20.0, -10.0, 0.0));
        Assert::AreEqual(-17.0, Wrap(-20.0, -10.0, 13.0));
        Assert::AreEqual(-12.0, Wrap(-20.0, -10.0, -22.0));
        Assert::AreEqual(-20.0, Wrap(-20.0, -10.0, -30.0));
        Assert::AreEqual(-13.0, Wrap(-20.0, -10.0, -33.0));
    }

};

}
}
