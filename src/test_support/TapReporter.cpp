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

#include "TapReporter.h"
#include <iostream>

TapReporter::TapReporter(std::ostream& out, int plan) : out_(out), plan_(plan)
{
    out_ << "1.." << plan << std::endl;
}

void TapReporter::Ok(const std::string& description)
{
    ++numTests_;
    ++numPass_;
    out_ << "ok " << numTests_ << " " << description << std::endl;
}

void TapReporter::NotOk(const std::string& description)
{
    ++numTests_;
    ++numFail_;
    out_ << "not ok " << numTests_ << " " << description << std::endl;
}

void TapReporter::Diagnostic(const std::string& text)
{
    out_ << "# " << text << std::endl;
}

bool TapReporter::IsFail()
{
    return (numFail_ > 0) || (numTests_ != plan_);
}

void TapReporter::PrintSummary()
{
    out_ << "# tests " << numTests_ << std::endl;
    out_ << "# pass  " << numPass_ << std::endl;
    if (numFail_ > 0) {
        out_ << "# fail  " << numFail_ << std::endl;
    }
    if (numTests_ != plan_) {
        out_ << "# unexpected number of tests" << std::endl;
    }
    if (!IsFail()) {
        out_ << "# ok" << std::endl;
    }
}
