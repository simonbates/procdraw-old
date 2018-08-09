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
    const std::vector<std::string>& Messages() const;

private:
    std::vector<std::string> msgs;
    void TestFunction(pugi::xml_node functionDoc,
                      int& numTests,
                      int& numFailed);
    void TestExample(pugi::xml_node example,
                     Interpreter& interpreter,
                     const char* functionName,
                     int& numTests,
                     int& numFailed);
};

} // namespace Tests
} // namespace Procdraw
