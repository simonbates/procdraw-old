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

#ifndef PROCDRAW_PROCDRAWMATH_H
#define PROCDRAW_PROCDRAWMATH_H

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

#endif
