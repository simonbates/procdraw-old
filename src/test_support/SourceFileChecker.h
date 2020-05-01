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

#pragma once

#include <istream>
#include <string>
#include <vector>

class SourceFileChecker {
public:
    bool Check(std::istream& in, const std::string& prefix, bool enablePragmaOnceCheck = false);
    std::string GetExpected();
    std::string GetActual();

private:
    void ReadNextLine(std::istream& in);
    bool CheckCopyright(std::istream& in);
    bool CheckApache2Header(std::istream& in);
    bool CheckPragmaOnce(std::istream& in);
    static std::vector<std::string> apache2Header_;
    std::string prefix_;
    std::string line_;
    std::string expected_;
    std::string actual_;
};
