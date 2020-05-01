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

#include "SourceFileChecker.h"
#include <string>
#include <vector>

std::vector<std::string> SourceFileChecker::apache2Header_{
    "",
    " Licensed under the Apache License, Version 2.0 (the \"License\");",
    " you may not use this file except in compliance with the License.",
    " You may obtain a copy of the License at",
    "",
    "     http://www.apache.org/licenses/LICENSE-2.0",
    "",
    " Unless required by applicable law or agreed to in writing, software",
    " distributed under the License is distributed on an \"AS IS\" BASIS,",
    " WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
    " See the License for the specific language governing permissions and",
    " limitations under the License."};

bool SourceFileChecker::Check(std::istream& in,
                              const std::string& prefix,
                              bool enablePragmaOnceCheck)
{
    prefix_ = prefix;
    ReadNextLine(in);
    return CheckCopyright(in)
           && CheckApache2Header(in)
           && (enablePragmaOnceCheck ? CheckPragmaOnce(in) : true);
}

std::string SourceFileChecker::GetExpected()
{
    return expected_;
}

std::string SourceFileChecker::GetActual()
{
    return actual_;
}

void SourceFileChecker::ReadNextLine(std::istream& in)
{
    std::getline(in, line_);
}

bool SourceFileChecker::CheckCopyright(std::istream& in)
{
    // TODO: C++20 will have starts_with
    if (line_.rfind(prefix_ + " Copyright ", 0) != 0) {
        expected_ = prefix_ + " Copyright ...";
        actual_ = line_;
        return false;
    }
    ReadNextLine(in);
    // TODO: C++20 will have starts_with
    while (line_.rfind(prefix_ + " Copyright ", 0) == 0) {
        ReadNextLine(in);
    }
    return true;
}

bool SourceFileChecker::CheckApache2Header(std::istream& in)
{
    for (auto headerLine : apache2Header_) {
        if (line_ != prefix_ + headerLine) {
            expected_ = prefix_ + headerLine;
            actual_ = line_;
            return false;
        }
        ReadNextLine(in);
    }
    return true;
}

bool SourceFileChecker::CheckPragmaOnce(std::istream& in)
{
    while (line_.empty()) {
        ReadNextLine(in);
    }
    if (line_ != "#pragma once") {
        expected_ = "#pragma once";
        actual_ = line_;
        return false;
    }
    else {
        return true;
    }
}
