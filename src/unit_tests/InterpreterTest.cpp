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

TEST(InterpreterTest, LookupSymbol)
{
    Interpreter interpreter;
    SymbolHandle foo1 = interpreter.SymbolRef("foo");
    ASSERT_EQ("foo", interpreter.SymbolName(foo1));
    SymbolHandle foo2 = interpreter.SymbolRef("foo");
    ASSERT_EQ(foo1, foo2);
}

TEST(InterpreterTest, SymbolsHaveInitialValueOfNone)
{
    Interpreter interpreter;
    SymbolHandle foo = interpreter.SymbolRef("foo");
    ASSERT_EQ(ObjectType::None, interpreter.SymbolValue(foo).Type());
}

TEST(InterpreterTest, SetAndGetSymbolValue)
{
    Interpreter interpreter;
    SymbolHandle foo = interpreter.SymbolRef("foo");
    ASSERT_EQ(ObjectType::None, interpreter.SymbolValue(foo).Type());
    interpreter.SetSymbolValue(foo, 42);
    Object val = interpreter.SymbolValue(foo);
    ASSERT_EQ(ObjectType::Integer, val.Type());
    ASSERT_EQ(42, val.GetInteger());
}

TEST(InterpreterTest, EvalBoolean)
{
    Interpreter interpreter;
    EXPECT_TRUE(interpreter.Eval(true).GetBoolean());
    EXPECT_FALSE(interpreter.Eval(false).GetBoolean());
}

TEST(InterpreterTest, EvalInteger)
{
    Interpreter interpreter;
    ASSERT_EQ(42, interpreter.Eval(42).GetInteger());
}

TEST(InterpreterTest, EvalNone)
{
    Interpreter interpreter;
    ASSERT_EQ(ObjectType::None, interpreter.Eval(Object::None()).Type());
}

TEST(InterpreterTest, EvalSymbol)
{
    Interpreter interpreter;
    SymbolHandle foo = interpreter.SymbolRef("foo");
    interpreter.SetSymbolValue(foo, 42);
    ASSERT_EQ(42, interpreter.Eval(Object::MakeSymbolHandle(foo)).GetInteger());
}

TEST(InterpreterTest, EvalSumFunction)
{
    Interpreter interpreter;
    EXPECT_EQ(0, interpreter.Eval(interpreter.Read("(+)")).GetInteger());
    EXPECT_EQ(0, interpreter.Eval(interpreter.Read("(+ 0)")).GetInteger());
    EXPECT_EQ(2, interpreter.Eval(interpreter.Read("(+ 2)")).GetInteger());
    EXPECT_EQ(5, interpreter.Eval(interpreter.Read("(+ 2 3)")).GetInteger());
    EXPECT_EQ(9, interpreter.Eval(interpreter.Read("(+ 2 3 4)")).GetInteger());
}
