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
#include <catch.hpp>

using namespace Procdraw;

TEST_CASE("Print Bool")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Print(true) == "true");
    REQUIRE(interpreter.Print(false) == "false");
}

TEST_CASE("Print Int")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Print(42) == "42");
}

TEST_CASE("Print List")
{
    Interpreter interpreter;

    REQUIRE(interpreter.Print(Object::EmptyList()) == "()");

    REQUIRE(interpreter.Print(Cons(42, nullptr)) == "(42)");

    REQUIRE(interpreter.Print(Cons(1, Cons(2, Cons(3, nullptr)))) == "(1 2 3)");

    REQUIRE(interpreter.Print(Cons(
                Cons(1, nullptr),
                Cons(Cons(2, nullptr), nullptr)))
            == "((1) (2))");
}

TEST_CASE("Print None")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Print(Object::None()) == "none");
}

TEST_CASE("Print Symbol")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Print(Object::MakeSymbolHandle(interpreter.SymbolRef("foo"))) == "foo");
}
