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

TEST_CASE("Lookup Symbol")
{
    Interpreter interpreter;
    SymbolHandle foo1 = interpreter.SymbolRef("foo");
    REQUIRE(interpreter.SymbolName(foo1) == "foo");
    SymbolHandle foo2 = interpreter.SymbolRef("foo");
    REQUIRE(foo1 == foo2);
}

TEST_CASE("Symbols have an initial value of None")
{
    Interpreter interpreter;
    SymbolHandle foo = interpreter.SymbolRef("foo");
    REQUIRE(interpreter.SymbolValue(foo).Type() == ObjectType::None);
}

TEST_CASE("Set and get Symbol value")
{
    Interpreter interpreter;
    SymbolHandle foo = interpreter.SymbolRef("foo");
    REQUIRE(interpreter.SymbolValue(foo).Type() == ObjectType::None);
    interpreter.SetSymbolValue(foo, 42);
    Object val = interpreter.SymbolValue(foo);
    REQUIRE(val.Type() == ObjectType::Integer);
    REQUIRE(val.GetInteger() == 42);
}

TEST_CASE("Eval Boolean")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Eval(true).GetBoolean());
    REQUIRE_FALSE(interpreter.Eval(false).GetBoolean());
}

TEST_CASE("Eval Integer")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Eval(42).GetInteger() == 42);
}

TEST_CASE("Eval None")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Eval(Object::None()).Type() == ObjectType::None);
}

TEST_CASE("Eval Symbol")
{
    Interpreter interpreter;
    SymbolHandle foo = interpreter.SymbolRef("foo");
    interpreter.SetSymbolValue(foo, 42);
    REQUIRE(interpreter.Eval(Object::MakeSymbolHandle(foo)).GetInteger() == 42);
}

TEST_CASE("Eval sum function")
{
    Interpreter interpreter;
    REQUIRE(interpreter.Eval(interpreter.Read("(+)")).GetInteger() == 0);
    REQUIRE(interpreter.Eval(interpreter.Read("(+ 0)")).GetInteger() == 0);
    REQUIRE(interpreter.Eval(interpreter.Read("(+ 2)")).GetInteger() == 2);
    REQUIRE(interpreter.Eval(interpreter.Read("(+ 2 3)")).GetInteger() == 5);
    REQUIRE(interpreter.Eval(interpreter.Read("(+ 2 3 4)")).GetInteger() == 9);
}
