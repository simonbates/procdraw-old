/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DocTester.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace Procdraw {
namespace Tests {

// clang-format off

class DocTesterTests : public ::testing::Test {
protected:

    void ExpectFailure(const std::string& filename,
                       int expectedNumTests,
                       const std::string& expectedMessage)
    {
        DocTester tester;
        const std::string filepath = TestFilepath(filename);
        EXPECT_FALSE(tester.RunTests(filepath.c_str(), expectedNumTests));
        EXPECT_TRUE(tester.Messages().size() == 1);
        EXPECT_EQ(expectedMessage, tester.Messages().at(0));
    }

    std::string TestFilepath(const std::string& filename)
    {
        return PROCDRAW_TEST_DATA_DIR + std::string("/") + filename;
    }

};

TEST_F(DocTesterTests, NonExistingFileThrowsException)
{
    DocTester tester;
    EXPECT_THROW(tester.RunTests("NON_EXISTING_FILE", 0), std::invalid_argument);
}

TEST_F(DocTesterTests, Failing1)
{
    ExpectFailure("function_docs_failing_1.xml", 1,
                  "FUNCTION: failing-1 EXPR: (+ 1) EXPECTED: 0 ACTUAL: 1");
}

TEST_F(DocTesterTests, Failing2)
{
    ExpectFailure("function_docs_failing_2.xml", 2,
                  "FUNCTION: failing-2 EXPR: (+ 2) EXPECTED: 0 ACTUAL: 2");
}

TEST_F(DocTesterTests, Failing3)
{
    ExpectFailure("function_docs_failing_3.xml", 2,
                  "FUNCTION: failing-3 EXPR: (+ 3) EXPECTED: 0 ACTUAL: 3");
}

TEST_F(DocTesterTests, Failing4)
{
    ExpectFailure("function_docs_failing_4.xml", 2,
                  "FUNCTION: failing-4b EXPR: (+ 4) EXPECTED: 0 ACTUAL: 4");
}

TEST_F(DocTesterTests, TooFewTests)
{
    ExpectFailure("function_docs_ok.xml", 4,
                  "EXPECTED 4 TESTS BUT 3 WERE RUN");
}

TEST_F(DocTesterTests, TooManyTests)
{
    ExpectFailure("function_docs_ok.xml", 2,
                  "EXPECTED 2 TESTS BUT 3 WERE RUN");
}

TEST_F(DocTesterTests, Passing)
{
    DocTester tester;
    EXPECT_TRUE(tester.RunTests(TestFilepath("function_docs_ok.xml").c_str(), 3));
    EXPECT_TRUE(tester.Messages().empty());
}

}
}
