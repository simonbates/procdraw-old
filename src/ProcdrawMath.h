/* Copyright (c) 2015, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

namespace Procdraw {

template <typename T>
T Clamp(T value, T lower, T upper)
{
    if (value < lower) {
        return lower;
    }
    else if (value > upper) {
        return upper;
    }
    return value;
}

bool ApproximatelyEqual(double a, double b, double epsilon);
double Lerp(double start, double stop, double val);
double
MapRange(double start1, double stop1, double start2, double stop2, double val);
double Norm(double start, double stop, double val);
int PowerOf2Gte(int n);
double Wrap(double start, double stop, double val);

} // namespace Procdraw
