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

#include "../test_support/SourceFileChecker.h"
#include "../test_support/TapReporter.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

bool isCppFile(const fs::directory_entry& file)
{
    return file.is_regular_file()
           && (file.path().extension() == ".cpp"
               || file.path().extension() == ".h");
}

bool isCppHeader(const fs::directory_entry& file)
{
    return isCppFile(file) && (file.path().extension() != ".cpp");
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        return 1;
    }

    int expectedNumFiles = std::stoi(argv[1]);
    std::string directory{argv[2]};

    TapReporter reporter{std::cout, expectedNumFiles};
    SourceFileChecker checker;

    for (const auto& file : fs::recursive_directory_iterator(directory)) {
        if (isCppFile(file)) {
            std::ifstream fileIn{file.path().string()};
            if (checker.Check(fileIn, "//", isCppHeader(file))) {
                reporter.Ok(file.path().string());
            }
            else {
                reporter.NotOk(file.path().string());
                reporter.Diagnostic("Expected: " + checker.GetExpected());
                reporter.Diagnostic("Actual: " + checker.GetActual());
            }
        }
    }

    reporter.PrintSummary();

    if (reporter.IsFail()) {
        return 1;
    }

    return 0;
}
