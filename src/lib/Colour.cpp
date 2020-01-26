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

#include "Colour.h"

namespace Procdraw {

// Foley and van Dam Fig. 13.34
// h [0, 360)
// s [0, 1]
// v [0, 1]
std::tuple<float, float, float> Hsv2rgb(float h, float s, float v)
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    if (s == 0) {
        r = v;
        g = v;
        b = v;
    }
    else {
        if (h == 360) {
            h = 0;
        }
        h = h / 60;
        int i = static_cast<int>(h);
        float f = h - i;
        float p = v * (1 - s);
        float q = v * (1 - (s * f));
        float t = v * (1 - (s * (1 - f)));
        switch (i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
        }
    }

    return std::make_tuple(r, g, b);
}
} // namespace Procdraw
