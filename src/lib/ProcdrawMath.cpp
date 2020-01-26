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
