/* Copyright (c) 2014, 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <tuple>

namespace Procdraw {

std::tuple<float, float, float> Hsv2rgb(float h, float s, float v);
}
