/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "DocTester.h"
#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Procdraw {
namespace Tests {

bool DocTester::RunTests(const char* filename, int expectedNumTests)
{
    msgs.clear();

    pugi::xml_document doc;
    auto result = doc.load_file(filename);
    if (!result) {
        msgs.push_back(std::string("Error loading file: ") + filename);
        return false;
    }

    int numTests = 0;
    int numFailed = 0;

    auto functionDocs = doc.child("function-docs");
    for (auto functionDoc : functionDocs.children("function-doc")) {
        TestFunction(functionDoc, numTests, numFailed);
    }

    bool passed = (numFailed == 0);

    // And fail also if we didn't run the expected number of tests
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

void DocTester::TestFunction(pugi::xml_node functionDoc,
                             int& numTests,
                             int& numFailed)
{
    Interpreter interpreter;
    const char* functionName = functionDoc.attribute("name").value();
    for (auto example : functionDoc.child("examples").children("ex")) {
        TestExample(example, interpreter, functionName, numTests, numFailed);
    }
}

void DocTester::TestExample(pugi::xml_node example,
                            Interpreter& interpreter,
                            const char* functionName,
                            int& numTests,
                            int& numFailed)
{
    const char* expr = example.attribute("expr").value();
    const char* expectedValue = example.attribute("value").value();
    interpreter.Read(expr);
    interpreter.Eval();
    std::string actual = interpreter.PrintToString();
    ++numTests;
    if (actual != expectedValue) {
        ++numFailed;
        msgs.push_back(std::string("FUNCTION: ") + functionName
                       + " EXPR: " + expr
                       + " EXPECTED: " + expectedValue
                       + " ACTUAL: " + actual);
    }
}

} // namespace Tests
} // namespace Procdraw
