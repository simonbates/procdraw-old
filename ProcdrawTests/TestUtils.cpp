/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

void AssertStackSize(StackIndexType expected, const Interpreter& interpreter)
{
    Assert::AreEqual(expected, interpreter.StackSize());
}

void AssertStackSize(StackIndexType expected, const VirtualMachine& vm)
{
    Assert::AreEqual(expected, vm.StackSize());
}

} // namespace Tests
} // namespace Procdraw
