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
    messages.clear();
    int numTests = 0;
    int numPassed = 0;

    pugi::xml_document doc;
    auto result = doc.load_file(filename);

    if (!result) {
        messages.push_back(std::string("Error loading file: ") + filename);
        return false;
    }

    auto functionDocs = doc.child("function-docs");
    for (auto functionDoc : functionDocs.children("function-doc")) {
        TestFunction(functionDoc, &numTests, &numPassed);
    }

    // Did all tests pass?
    if (numTests != numPassed) {
        return false;
    }

    if (expectedNumTests != numTests) {
        messages.push_back("EXPECTED " + std::to_string(expectedNumTests)
                           + " TESTS BUT " + std::to_string(numTests)
                           + " WERE RUN");
        return false;
    }

    return true;
}

std::vector<std::string> DocTester::GetMessages()
{
    return messages;
}

void DocTester::TestFunction(pugi::xml_node functionDoc,
                             int* numTests,
                             int* numPassed)
{
    Interpreter interpreter;
    const char* functionName = functionDoc.attribute("name").value();
    for (auto example : functionDoc.child("examples").children("ex")) {
        TestExample(example, &interpreter, functionName, numTests, numPassed);
    }
}

void DocTester::TestExample(pugi::xml_node example,
                            Interpreter* interpreter,
                            const char* functionName,
                            int* numTests,
                            int* numPassed)
{
    const char* expr = example.attribute("expr").value();
    const char* expectedValue = example.attribute("value").value();
    interpreter->Read(expr);
    interpreter->Eval();
    std::string result = interpreter->PrintToString();
    *numTests = *numTests + 1;
    if (result == expectedValue) {
        *numPassed = *numPassed + 1;
    }
    else {
        messages.push_back(std::string("FUNCTION: ") + functionName
                           + " EXPR: " + expr + " EXPECTED: "
                           + expectedValue + " ACTUAL: " + result);
    }
}
} // namespace Tests
} // namespace Procdraw
