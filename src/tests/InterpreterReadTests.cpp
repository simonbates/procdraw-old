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

TEST_CASE("Read true keyword")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("true");
    REQUIRE(obj.Type() == ObjectType::Boolean);
    REQUIRE(obj.GetBoolean());
}

TEST_CASE("Read false keyword")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("false");
    REQUIRE(obj.Type() == ObjectType::Boolean);
    REQUIRE_FALSE(obj.GetBoolean());
}

TEST_CASE("Read none keyword")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("none");
    REQUIRE(obj.Type() == ObjectType::None);
}

TEST_CASE("Read integer without sign")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("42");
    REQUIRE(obj.Type() == ObjectType::Integer);
    REQUIRE(obj.GetInteger() == 42);
}

TEST_CASE("Read integer with positive sign")
{
    Interpreter interprepter;
    Object obj = interprepter.Read("+42");
    REQUIRE(obj.Type() == ObjectType::Integer);
    REQUIRE(obj.GetInteger() == 42);
}

TEST_CASE("Read empty list")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("()");
    REQUIRE(obj.Type() == ObjectType::ListPtr);
    REQUIRE(obj.GetListPtr() == nullptr);
}

TEST_CASE("Read a list with one item")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("(42)");
    REQUIRE(obj.Type() == ObjectType::ListPtr);
    REQUIRE(obj.GetListPtr()->First().GetInteger() == 42);
    REQUIRE((obj.GetListPtr()->Rest() == nullptr));
}

// +---+  +---+  +---+  +---+
// |1|*|->|2|*|->|3|*|->|4|/|
// +---+  +---+  +---+  +---+
//
TEST_CASE("Read a list with multiple items")
{
    Interpreter interpreter;
    ListPtr lst = interpreter.Read("(1 2 3 4)").GetListPtr();
    REQUIRE(lst->First().GetInteger() == 1);
    REQUIRE(lst->Rest()->First().GetInteger() == 2);
    REQUIRE(lst->Rest()->Rest()->First().GetInteger() == 3);
    REQUIRE(lst->Rest()->Rest()->Rest()->First().GetInteger() == 4);
    REQUIRE(lst->Rest()->Rest()->Rest()->Rest() == nullptr);
}

// +---+  +---+  +---+
// |*|*|->|3|*|->|4|/|
// +---+  +---+  +---+
//  |
//  v
// +---+  +---+
// |1|*|->|2|/|
// +---+  +---+
//
TEST_CASE("Read a list with an embedded list")
{
    Interpreter interpreter;
    ListPtr lst = interpreter.Read("((1 2) 3 4)").GetListPtr();
    REQUIRE(lst->First().GetListPtr()->First().GetInteger() == 1);
    REQUIRE(lst->First().GetListPtr()->Rest()->First().GetInteger() == 2);
    REQUIRE(lst->First().GetListPtr()->Rest()->Rest() == nullptr);
    REQUIRE(lst->Rest()->First().GetInteger() == 3);
    REQUIRE(lst->Rest()->Rest()->First().GetInteger() == 4);
    REQUIRE(lst->Rest()->Rest()->Rest() == nullptr);
}

TEST_CASE("Read star char as symbol")
{
    Interpreter interpreter;
    Object obj = interpreter.Read("*");
    REQUIRE(obj.Type() == ObjectType::SymbolHandle);
    REQUIRE(interpreter.SymbolName(obj.GetSymbolHandle()) == "*");
}

TEST_CASE("Read plus char as Symbol")
{
    Interpreter interpreter;
    ListPtr lst = interpreter.Read("(+ 42)").GetListPtr();
    REQUIRE(lst->First().Type() == ObjectType::SymbolHandle);
    REQUIRE(interpreter.SymbolName(lst->First().GetSymbolHandle()) == "+");
    REQUIRE(lst->Rest()->First().Type() == ObjectType::Integer);
    REQUIRE(lst->Rest()->First().GetInteger() == 42);
}
