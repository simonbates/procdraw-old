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
#include "ProcdrawDocs.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace Procdraw::Tests {

bool DocsTester::RunTests(const char* filename, int expectedNumTests)
{
    msgs.clear();
    int numTests = 0;
    int numPassed = 0;

    const ProcdrawDocs docs(filename);
    auto iter = docs.FunctionDocs();
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

const std::vector<std::string>& DocsTester::Messages() const
{
    return msgs;
}

void DocsTester::TestFunction(const FunctionDoc& functionDoc,
                              int& numTests,
                              int& numPassed)
{
    Interpreter interpreter;
    std::string functionName = functionDoc.Name();
    for (auto example : functionDoc.Examples()) {
        TestExample(example, interpreter, functionName, numTests, numPassed);
    }
}

void DocsTester::TestExample(const FunctionExample& example,
                             Interpreter& interpreter,
                             const std::string& functionName,
                             int& numTests,
                             int& numPassed)
{
    std::string actual = interpreter.Print(interpreter.Eval(
        interpreter.Read(example.Expression())));
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

} // namespace Procdraw::Tests
