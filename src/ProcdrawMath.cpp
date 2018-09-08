/* Copyright (c) 2015, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "ProcdrawMath.h"
#include <cmath>

namespace Procdraw {

bool ApproximatelyEqual(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

double Lerp(double start, double stop, double val)
{
    return (1 - val) * start + val * stop;
}

double
MapRange(double start1, double stop1, double start2, double stop2, double val)
{
    return start2 + ((val - start1) * (stop2 - start2)) / (stop1 - start1);
}

double Norm(double start, double stop, double val)
{
    return (val - start) / (stop - start);
}

int PowerOf2Gte(int n)
{
    if (n <= 0) {
        return 0;
    }
    int powerOf2 = 1;
    while (powerOf2 < n) {
        powerOf2 *= 2;
    }
    return powerOf2;
}

double Wrap(double start, double stop, double val)
{
    return val - floor((val - start) / (stop - start)) * (stop - start);
}

} // namespace Procdraw
