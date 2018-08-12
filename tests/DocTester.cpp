/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "DocTester.h"
#include "ProcdrawManual.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace Procdraw {
namespace Tests {

bool DocTester::RunTests(const char* filename, int expectedNumTests)
{
    msgs.clear();
    int numTests = 0;
    int numPassed = 0;

    const ProcdrawManual man(filename);
    auto iter = man.FunctionDocs();
    while (iter.HasNext()) {
        TestFunction(iter.Next(), numTests, numPassed);
    }

    if (msgs.size() != (numTests - numPassed)) {
        throw std::logic_error("msgs.size() != (numTests - numPassed)");
    }

    bool passed = (numPassed == numTests);

    if (numTests != expectedNumTests) {
        passed = false;
        msgs.push_back("EXPECTED " + std::to_string(expectedNumTests)
                       + " TESTS BUT " + std::to_string(numTests)
                       + " WERE RUN");
    }

    return passed;
}

const std::vector<std::string>& DocTester::Messages() const
{
    return msgs;
}

void DocTester::TestFunction(const FunctionDoc& functionDoc,
                             int& numTests,
                             int& numPassed)
{
    Interpreter interpreter;
    std::string functionName = functionDoc.Name();
    for (auto example : functionDoc.Examples()) {
        TestExample(example, interpreter, functionName, numTests, numPassed);
    }
}

void DocTester::TestExample(const FunctionExample& example,
                            Interpreter& interpreter,
                            const std::string& functionName,
                            int& numTests,
                            int& numPassed)
{
    interpreter.Read(example.Expression());
    interpreter.Eval();
    std::string actual = interpreter.PrintToString();
    ++numTests;
    std::string expectedValue = example.Value();
    if (actual == expectedValue) {
        ++numPassed;
    }
    else {
        msgs.push_back(std::string("FUNCTION: ") + functionName
                       + " EXPR: " + example.Expression()
                       + " EXPECTED: " + expectedValue
                       + " ACTUAL: " + actual);
    }
}

} // namespace Tests
} // namespace Procdraw
