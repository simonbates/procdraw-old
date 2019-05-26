/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "Interpreter.h"
#include "PrintTo.h"
#include "TestUtils.h"

namespace Procdraw {
namespace Tests {

// clang-format off

TEST(InterpreterReadTests, ReadTrue)
{
    Interpreter interpreter;
    interpreter.Read("true");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Boolean, interpreter.Type());
    EXPECT_TRUE(interpreter.PopBoolean());
}

TEST(InterpreterReadTests, ReadFalse)
{
    Interpreter interpreter;
    interpreter.Read("false");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Boolean, interpreter.Type());
    EXPECT_FALSE(interpreter.PopBoolean());
}

TEST(InterpreterReadTests, ReadEmptyListAsNil)
{
    Interpreter interpreter;
    interpreter.Read("()");
    CheckStackSize(1, interpreter);
    EXPECT_TRUE(interpreter.IsNull());
    interpreter.Drop();
    CheckStackSize(0, interpreter);
    interpreter.Read(" ( ) ");
    CheckStackSize(1, interpreter);
    EXPECT_TRUE(interpreter.IsNull());
}

TEST(InterpreterReadTests, ReadNil)
{
    Interpreter interpreter;
    interpreter.Read("nil");
    CheckStackSize(1, interpreter);
    EXPECT_TRUE(interpreter.IsNull());
}

TEST(InterpreterReadTests, ReadList)
{
    Interpreter interpreter;
    interpreter.Read("(a b c)");                             // (a b c)
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    interpreter.Next();                                      // (b c) a
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("a"), interpreter.PopSymbol());    // (b c)
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    interpreter.Next();                                      // (c) b
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("b"), interpreter.PopSymbol());    // (c)
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    interpreter.Next();                                     // nil c
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("c"), interpreter.PopSymbol());   // nil
    EXPECT_TRUE(interpreter.IsNull());
    CheckStackSize(1, interpreter);
}

TEST(InterpreterReadTests, ReadDottedPair)
{
    Interpreter interpreter;
    interpreter.Read("(a . b)");                            // (a . b)
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    interpreter.Next();                                     // b a
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("a"), interpreter.PopSymbol());   // b
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("b"), interpreter.PopSymbol());
    CheckStackSize(0, interpreter);
}

TEST(InterpreterReadTests, ReadRealWithoutSign)
{
    Interpreter interpreter;
    interpreter.Read("42");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Real, interpreter.Type());
    EXPECT_EQ(42.0, interpreter.PopReal());
}

TEST(InterpreterReadTests, ReadNegativeReal)
{
    Interpreter interpreter;
    interpreter.Read("-42");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Real, interpreter.Type());
    EXPECT_EQ(-42.0, interpreter.PopReal());
}

TEST(InterpreterReadTests, ReadSymbol)
{
    Interpreter interpreter;
    interpreter.Read("foo");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("foo"), interpreter.PopSymbol());
}

TEST(InterpreterReadTests, ReadPlusSymbol)
{
    Interpreter interpreter;
    interpreter.Read("+");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("+"), interpreter.PopSymbol());
}

TEST(InterpreterReadTests, ReadHyphenMinusSymbol)
{
    Interpreter interpreter;
    interpreter.Read("-");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::SymbolPtr, interpreter.Type());
    EXPECT_EQ(std::string("-"), interpreter.PopSymbol());
}

}
}
