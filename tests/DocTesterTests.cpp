/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "DocTester.h"
#include <stdexcept>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

// clang-format off

TEST_CLASS(DocTesterTests) {
public:

    void ExpectFailure(const std::string& filename,
                       int expectedNumTests,
                       const std::string& expectedMessage)
    {
        DocTester tester;
        const std::string filepath = TestFilepath(filename);
        Assert::IsFalse(tester.RunTests(filepath.c_str(), expectedNumTests));
        Assert::IsTrue(tester.Messages().size() == 1);
        Assert::AreEqual(expectedMessage, tester.Messages().at(0));
    }

    std::string TestFilepath(const std::string& filename)
    {
        return PROCDRAW_TEST_DATA_DIR + std::string("/") + filename;
    }

    TEST_METHOD(NonExistingFileThrowsException)
    {
        DocTester tester;
        Assert::ExpectException<std::invalid_argument>([&tester]{
            tester.RunTests("NON_EXISTING_FILE", 0);
        });
    }

    TEST_METHOD(Failing1)
    {
        ExpectFailure("function_docs_failing_1.xml", 1,
                      "FUNCTION: failing-1 EXPR: (+ 1) EXPECTED: 0 ACTUAL: 1");
    }

    TEST_METHOD(Failing2)
    {
        ExpectFailure("function_docs_failing_2.xml", 2,
                      "FUNCTION: failing-2 EXPR: (+ 2) EXPECTED: 0 ACTUAL: 2");
    }

    TEST_METHOD(Failing3)
    {
        ExpectFailure("function_docs_failing_3.xml", 2,
                      "FUNCTION: failing-3 EXPR: (+ 3) EXPECTED: 0 ACTUAL: 3");
    }

    TEST_METHOD(Failing4)
    {
        ExpectFailure("function_docs_failing_4.xml", 2,
                      "FUNCTION: failing-4b EXPR: (+ 4) EXPECTED: 0 ACTUAL: 4");
    }

    TEST_METHOD(TooFewTests)
    {
        ExpectFailure("function_docs_ok.xml", 4,
                      "EXPECTED 4 TESTS BUT 3 WERE RUN");
    }

    TEST_METHOD(TooManyTests)
    {
        ExpectFailure("function_docs_ok.xml", 2,
                      "EXPECTED 2 TESTS BUT 3 WERE RUN");
    }

    TEST_METHOD(Passing)
    {
        DocTester tester;
        Assert::IsTrue(tester.RunTests(TestFilepath("function_docs_ok.xml").c_str(), 3));
        Assert::IsTrue(tester.Messages().empty());
    }

};

}
}
