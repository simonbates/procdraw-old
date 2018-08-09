/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "DocTester.h"
#include <Windows.h>
#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

// clang-format off

TEST_CLASS(FunctionDocsTests) {
public:

    TEST_METHOD(RunFunctionDocsTests)
    {
        const int expectedNumTests = 10;

        DocTester tester;
        bool passed = tester.RunTests(PROCDRAW_FUNCTION_DOCS_FILE,
                                      expectedNumTests);
        std::array<wchar_t, 1024> wmsg;
        for (auto message : tester.Messages()) {
            int c = MultiByteToWideChar(CP_UTF8, 0,
                                        (message + "\n").c_str(), -1,
                                        wmsg.data(), wmsg.size());
            Assert::IsTrue(c > 0);
            Logger::WriteMessage(wmsg.data());
        }
        Assert::IsTrue(passed);
    }

};

}
}
