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

#include "../lib/Interpreter.h"
#include <gtest/gtest.h>

using namespace Procdraw;

TEST(InterpreterPrintTest, PrintBool)
{
    Interpreter interpreter;
    EXPECT_EQ("true", interpreter.Print(true));
    EXPECT_EQ("false", interpreter.Print(false));
}

TEST(InterpreterPrintTest, PrintInt)
{
    Interpreter interpreter;
    EXPECT_EQ("42", interpreter.Print(42));
}

TEST(InterpreterPrintTest, PrintList)
{
    Interpreter interpreter;

    EXPECT_EQ("()", interpreter.Print(Object::EmptyList()));

    EXPECT_EQ("(42)", interpreter.Print(Cons(42, nullptr)));

    EXPECT_EQ("(1 2 3)", interpreter.Print(Cons(1, Cons(2, Cons(3, nullptr)))));

    EXPECT_EQ("((1) (2))",
              interpreter.Print(Cons(
                  Cons(1, nullptr),
                  Cons(Cons(2, nullptr), nullptr))));
}

TEST(InterpreterPrintTest, PrintNone)
{
    Interpreter interpreter;
    EXPECT_EQ("none", interpreter.Print(Object::None()));
}

TEST(InterpreterPrintTest, PrintSymbol)
{
    Interpreter interpreter;
    EXPECT_EQ("foo", interpreter.Print(Object::MakeSymbolHandle(interpreter.SymbolRef("foo"))));
}
