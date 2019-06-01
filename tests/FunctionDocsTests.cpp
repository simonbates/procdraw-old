/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "DocTester.h"

namespace Procdraw {
namespace Tests {

TEST(FunctionDocsTests, RunFunctionDocsTests)
{
    const int expectedNumTests = 15;

    DocTester tester;
    bool passed = tester.RunTests(PROCDRAW_FUNCTION_DOCS_FILE,
                                  expectedNumTests);
    EXPECT_TRUE(passed);
    for (auto message : tester.Messages()) {
        ADD_FAILURE() << message;
    }
}

} // namespace Tests
} // namespace Procdraw
