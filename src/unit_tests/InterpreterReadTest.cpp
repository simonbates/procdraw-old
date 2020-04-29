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

TEST(InterpreterReadTest, ReadTrueKeyword)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("true");
    ASSERT_EQ(ObjectType::Boolean, obj.Type());
    ASSERT_TRUE(obj.GetBoolean());
}

TEST(InterpreterReadTest, ReadFalseKeyword)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("false");
    ASSERT_EQ(ObjectType::Boolean, obj.Type());
    ASSERT_FALSE(obj.GetBoolean());
}

TEST(InterpreterReadTest, ReadNoneKeyword)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("none");
    ASSERT_EQ(ObjectType::None, obj.Type());
}

TEST(InterpreterReadTest, ReadIntegerWithoutSign)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("42");
    ASSERT_EQ(ObjectType::Integer, obj.Type());
    ASSERT_EQ(42, obj.GetInteger());
}

TEST(InterpreterReadTest, ReadIntegerWithPositiveSign)
{
    Interpreter interprepter;
    Object obj = interprepter.Read("+42");
    ASSERT_EQ(ObjectType::Integer, obj.Type());
    ASSERT_EQ(42, obj.GetInteger());
}

TEST(InterpreterReadTest, ReadEmptyList)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("()");
    ASSERT_EQ(ObjectType::ListPtr, obj.Type());
    ASSERT_EQ(nullptr, obj.GetListPtr());
}

TEST(InterpreterReadTest, ReadListWithOneItem)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("(42)");
    ASSERT_EQ(ObjectType::ListPtr, obj.Type());
    ASSERT_EQ(42, obj.GetListPtr()->First().GetInteger());
    ASSERT_EQ(nullptr, obj.GetListPtr()->Rest());
}

// +---+  +---+  +---+  +---+
// |1|*|->|2|*|->|3|*|->|4|/|
// +---+  +---+  +---+  +---+
//
TEST(InterpreterReadTest, ReadListWithMultipleItems)
{
    Interpreter interpreter;
    ListPtr lst = interpreter.Read("(1 2 3 4)").GetListPtr();
    ASSERT_EQ(1, lst->First().GetInteger());
    ASSERT_EQ(2, lst->Rest()->First().GetInteger());
    ASSERT_EQ(3, lst->Rest()->Rest()->First().GetInteger());
    ASSERT_EQ(4, lst->Rest()->Rest()->Rest()->First().GetInteger());
    ASSERT_EQ(nullptr, lst->Rest()->Rest()->Rest()->Rest());
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
TEST(InterpreterReadTest, ReadListWithEmbeddedList)
{
    Interpreter interpreter;
    ListPtr lst = interpreter.Read("((1 2) 3 4)").GetListPtr();
    ASSERT_EQ(1, lst->First().GetListPtr()->First().GetInteger());
    ASSERT_EQ(2, lst->First().GetListPtr()->Rest()->First().GetInteger());
    ASSERT_EQ(nullptr, lst->First().GetListPtr()->Rest()->Rest());
    ASSERT_EQ(3, lst->Rest()->First().GetInteger());
    ASSERT_EQ(4, lst->Rest()->Rest()->First().GetInteger());
    ASSERT_EQ(nullptr, lst->Rest()->Rest()->Rest());
}

TEST(InterpreterReadTest, ReadStarCharAsSymbol)
{
    Interpreter interpreter;
    Object obj = interpreter.Read("*");
    ASSERT_EQ(ObjectType::SymbolHandle, obj.Type());
    ASSERT_EQ("*", interpreter.SymbolName(obj.GetSymbolHandle()));
}

TEST(InterpreterReadTest, ReadPlusCharAsSymbol)
{
    Interpreter interpreter;
    ListPtr lst = interpreter.Read("(+ 42)").GetListPtr();
    ASSERT_EQ(ObjectType::SymbolHandle, lst->First().Type());
    ASSERT_EQ("+", interpreter.SymbolName(lst->First().GetSymbolHandle()));
    ASSERT_EQ(ObjectType::Integer, lst->Rest()->First().Type());
    ASSERT_EQ(42, lst->Rest()->First().GetInteger());
}
