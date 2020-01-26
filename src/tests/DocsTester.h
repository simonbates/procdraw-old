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

#ifndef PROCDRAW_DOCSTESTER_H
#define PROCDRAW_DOCSTESTER_H

#include "../lib/Interpreter.h"
#include "ProcdrawDocs.h"
#include <string>
#include <vector>

namespace Procdraw::Tests {

class DocsTester {
public:
    bool RunTests(const char* filename, int expectedNumTests);
    const std::vector<std::string>& Messages() const;

private:
    std::vector<std::string> msgs;
    void TestFunction(const FunctionDoc& functionDoc,
                      int& numTests,
                      int& numPassed);
    void TestExample(const FunctionExample& example,
                     Interpreter& interpreter,
                     const std::string& functionName,
                     int& numTests,
                     int& numPassed);
};

} // namespace Procdraw::Tests

#endif
