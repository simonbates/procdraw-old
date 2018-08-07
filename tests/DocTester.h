/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "Interpreter.h"
#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Procdraw {
namespace Tests {

class DocTester {
public:
    bool RunTests(const char* filename, int expectedNumTests);
    std::vector<std::string> GetMessages();

private:
    std::vector<std::string> messages;
    void TestFunction(pugi::xml_node functionDoc,
                      int* numTests,
                      int* numPassed);
    void TestExample(pugi::xml_node example,
                     Interpreter* interpreter,
                     const char* functionName,
                     int* numTests,
                     int* numPassed);
};
} // namespace Tests
} // namespace Procdraw
