/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "Interpreter.h"

namespace Procdraw {
namespace Tests {

void AssertStackSize(StackIndexType expected, const Interpreter& interpreter);
}
} // namespace Procdraw
