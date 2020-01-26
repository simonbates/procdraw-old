// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "DocsTester.h"
#include <catch.hpp>
#include <stdexcept>
#include <string>

static std::string TestFilepath(const std::string& filename)
{
    return PROCDRAW_TEST_DATA_DIR + std::string("/") + filename;
}

static void ExpectFailure(const std::string& filename,
                          int expectedNumTests,
                          const std::string& expectedMessage)
{
    Procdraw::Tests::DocsTester tester;
    const std::string filepath = TestFilepath(filename);
    REQUIRE_FALSE(tester.RunTests(filepath.c_str(), expectedNumTests));
    REQUIRE(tester.Messages().size() == 1);
    REQUIRE(tester.Messages().at(0) == expectedMessage);
}

TEST_CASE("Non-existing file throws exception")
{
    Procdraw::Tests::DocsTester tester;
    REQUIRE_THROWS_AS(tester.RunTests("NON_EXISTING_FILE", 0), std::invalid_argument);
}

TEST_CASE("Failing 1")
{
    ExpectFailure("function_docs_failing_1.xml", 1,
                  "FUNCTION: failing-1 EXPR: (+ 1) EXPECTED: 0 ACTUAL: 1");
}

TEST_CASE("Failing 2")
{
    ExpectFailure("function_docs_failing_2.xml", 2,
                  "FUNCTION: failing-2 EXPR: (+ 2) EXPECTED: 0 ACTUAL: 2");
}

TEST_CASE("Failing 3")
{
    ExpectFailure("function_docs_failing_3.xml", 2,
                  "FUNCTION: failing-3 EXPR: (+ 3) EXPECTED: 0 ACTUAL: 3");
}

TEST_CASE("Failing 4")
{
    ExpectFailure("function_docs_failing_4.xml", 2,
                  "FUNCTION: failing-4b EXPR: (+ 4) EXPECTED: 0 ACTUAL: 4");
}

TEST_CASE("Too few tests")
{
    ExpectFailure("function_docs_ok.xml", 4,
                  "EXPECTED 4 TESTS BUT 3 WERE RUN");
}

TEST_CASE("Too many tests")
{
    ExpectFailure("function_docs_ok.xml", 2,
                  "EXPECTED 2 TESTS BUT 3 WERE RUN");
}

TEST_CASE("Passing")
{
    Procdraw::Tests::DocsTester tester;
    REQUIRE(tester.RunTests(TestFilepath("function_docs_ok.xml").c_str(), 3));
    REQUIRE(tester.Messages().empty());
}
