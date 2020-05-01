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
#include <gtest/gtest.h>
#include <sstream>
#include <string>

const char goodCppSource[]{
    "// Copyright 2020 Some Name\n"
    "//\n"
    "// Licensed under the Apache License, Version 2.0 (the \"License\");\n"
    "// you may not use this file except in compliance with the License.\n"
    "// You may obtain a copy of the License at\n"
    "//\n"
    "//     http://www.apache.org/licenses/LICENSE-2.0\n"
    "//\n"
    "// Unless required by applicable law or agreed to in writing, software\n"
    "// distributed under the License is distributed on an \"AS IS\" BASIS,\n"
    "// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
    "// See the License for the specific language governing permissions and\n"
    "// limitations under the License.\n"
    "\n"
    "namespace Procdraw {\n"
    "}"};

const char goodCppMultipleCopyrightSource[]{
    "// Copyright 2020 Some Name1\n"
    "// Copyright 2020 Some Name2\n"
    "// Copyright 2020 Some Name3\n"
    "//\n"
    "// Licensed under the Apache License, Version 2.0 (the \"License\");\n"
    "// you may not use this file except in compliance with the License.\n"
    "// You may obtain a copy of the License at\n"
    "//\n"
    "//     http://www.apache.org/licenses/LICENSE-2.0\n"
    "//\n"
    "// Unless required by applicable law or agreed to in writing, software\n"
    "// distributed under the License is distributed on an \"AS IS\" BASIS,\n"
    "// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
    "// See the License for the specific language governing permissions and\n"
    "// limitations under the License.\n"
    "\n"
    "namespace Procdraw {\n"
    "}"};

const char goodHeaderSource[]{
    "// Copyright 2020 Some Name\n"
    "//\n"
    "// Licensed under the Apache License, Version 2.0 (the \"License\");\n"
    "// you may not use this file except in compliance with the License.\n"
    "// You may obtain a copy of the License at\n"
    "//\n"
    "//     http://www.apache.org/licenses/LICENSE-2.0\n"
    "//\n"
    "// Unless required by applicable law or agreed to in writing, software\n"
    "// distributed under the License is distributed on an \"AS IS\" BASIS,\n"
    "// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
    "// See the License for the specific language governing permissions and\n"
    "// limitations under the License.\n"
    "\n"
    "#pragma once\n"
    "\n"
    "namespace Procdraw {\n"
    "}"};

TEST(SourceFileCheckerTest, Empty)
{
    SourceFileChecker checker;
    EXPECT_FALSE(checker.Check(std::istringstream{""}, "//"));
    EXPECT_EQ("// Copyright ...", checker.GetExpected());
    EXPECT_EQ("", checker.GetActual());
}

TEST(SourceFileCheckerTest, NoCopyright)
{
    SourceFileChecker checker;
    EXPECT_FALSE(checker.Check(std::istringstream{"asdf"}, "//"));
    EXPECT_EQ("// Copyright ...", checker.GetExpected());
    EXPECT_EQ("asdf", checker.GetActual());
}

TEST(SourceFileCheckerTest, BadApache2Header)
{
    SourceFileChecker checker;
    std::istringstream badHeader{
        "// Copyright 2020 Some Name\n"
        "//\n"
        "// Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "// you may not use this file except in compliance with the License.\n"
        "// asdf\n"};
    EXPECT_FALSE(checker.Check(badHeader, "//"));
    EXPECT_EQ("// You may obtain a copy of the License at", checker.GetExpected());
    EXPECT_EQ("// asdf", checker.GetActual());
}

TEST(SourceFileCheckerTest, MissingPragmaOnce)
{
    SourceFileChecker checker;
    EXPECT_FALSE(checker.Check(std::istringstream{goodCppSource}, "//", true));
    EXPECT_EQ("#pragma once", checker.GetExpected());
    EXPECT_EQ("namespace Procdraw {", checker.GetActual());
}

TEST(SourceFileCheckerTest, GoodCpp)
{
    SourceFileChecker checker;
    EXPECT_TRUE(checker.Check(std::istringstream{goodCppSource}, "//"));
}

TEST(SourceFileCheckerTest, MultipleCopyright)
{
    SourceFileChecker checker;
    EXPECT_TRUE(checker.Check(std::istringstream{goodCppMultipleCopyrightSource}, "//"));
}

TEST(SourceFileCheckerTest, GoodHeader)
{
    SourceFileChecker checker;
    EXPECT_TRUE(checker.Check(std::istringstream{goodHeaderSource}, "//", true));
}
