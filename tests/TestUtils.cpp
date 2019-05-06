/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestUtils.h"
#include <gtest/gtest.h>

namespace Procdraw {
namespace Tests {

void CheckStackSize(StackIndexType expected, const Interpreter& interpreter)
{
    EXPECT_EQ(expected, interpreter.StackSize());
}

void CheckStackSize(StackIndexType expected, const VirtualMachine& vm)
{
    EXPECT_EQ(expected, vm.StackSize());
}

} // namespace Tests
} // namespace Procdraw
