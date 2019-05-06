/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "Interpreter.h"
#include "VirtualMachine.h"

namespace Procdraw {
namespace Tests {

void CheckStackSize(StackIndexType expected, const Interpreter& interpreter);
void CheckStackSize(StackIndexType expected, const VirtualMachine& vm);
} // namespace Tests
} // namespace Procdraw
