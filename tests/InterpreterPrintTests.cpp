/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "Interpreter.h"
#include "TestUtils.h"

namespace Procdraw {
namespace Tests {

TEST(InterpreterPrintTests, PrintToStringAtoms)
{
    Interpreter interpreter;

    interpreter.PushReal(42);
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("42"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);

    interpreter.PushBoolean(true);
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("true"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);

    interpreter.PushBoolean(false);
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("false"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);

    interpreter.PushSymbol("foo");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("foo"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);
}

TEST(InterpreterPrintTests, PrintToStringNil)
{
    Interpreter interpreter;
    interpreter.PushNil();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("nil"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);
}

TEST(InterpreterPrintTests, PrintToStringLists)
{
    Interpreter interpreter;

    interpreter.Read("(a b c)");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("(a b c)"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);

    interpreter.Read("(lambda (param1 param2) (call1) (call2 arg1 arg2 arg3))");
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("(lambda (param1 param2) (call1) (call2 arg1 arg2 arg3))"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);
}

TEST(InterpreterPrintTests, PrintToStringDottedPair)
{
    Interpreter interpreter;
    interpreter.PushSymbol("b");
    interpreter.PushSymbol("a");
    interpreter.Cons();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(std::string("(a . b)"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);
}

} // namespace Tests
} // namespace Procdraw
