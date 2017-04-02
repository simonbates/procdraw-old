#pragma once

#include <tuple>

namespace procdraw {

std::tuple<float, float, float> hsv2rgb(float h, float s, float v);
}
