/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "Interpreter.h"
#include "PrintTo.h"
#include "TestUtils.h"

// TODO: Do as many tests as possible with the DocTester
// TODO: Tests for (set var expr)

namespace Procdraw {
namespace Tests {

// clang-format off

TEST(InterpreterTests, EvalSumExpression)
{
    Interpreter interpreter;
    CheckStackSize(0, interpreter);
    interpreter.Read("(+ 1 (+ 2 3))");
    interpreter.Eval();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Real, interpreter.Type());
    EXPECT_EQ(6.0, interpreter.PopReal());
}

TEST(InterpreterTests, AssocFindsMatch)
{
    Interpreter interpreter;
    CheckStackSize(0, interpreter);
    interpreter.PushSymbol("b");
    CheckStackSize(1, interpreter);
    interpreter.Read("((a . 10) (b . 11) (c . 12))");
    CheckStackSize(2, interpreter);
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    interpreter.Assoc();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    // TODO: Rather than checking printed value, check is same object with Eq
    //       Read Dup Cadr Swap -- Should put (b . 11) under the alist
    EXPECT_EQ(std::string("(b . 11)"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);
}

TEST(InterpreterTests, AssocNilOnMatchNotFound)
{
    Interpreter interpreter;
    CheckStackSize(0, interpreter);
    interpreter.PushSymbol("d");
    CheckStackSize(1, interpreter);
    interpreter.Read("((a . 10) (b . 11) (c . 12))");
    CheckStackSize(2, interpreter);
    EXPECT_EQ(ObjType::ConsPtr, interpreter.Type());
    interpreter.Assoc();
    CheckStackSize(1, interpreter);
    EXPECT_TRUE(interpreter.IsNull());
}

TEST(InterpreterTests, EvalLambdaMakesExpr)
{
    Interpreter interpreter;
    CheckStackSize(0, interpreter);
    interpreter.Read("(lambda (x) (+ x 1))");
    interpreter.Eval();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Expr, interpreter.Type());
    EXPECT_EQ(std::string("(lambda (x) (+ x 1))"), interpreter.PrintToString());
    CheckStackSize(0, interpreter);
}

TEST(InterpreterTests, EvalLambdaExpr)
{
    Interpreter interpreter;
    CheckStackSize(0, interpreter);
    interpreter.Read("((lambda (x y) (+ (- x y) 1)) 11 5)");
    interpreter.Eval();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Real, interpreter.Type());
    EXPECT_EQ(7.0, interpreter.PopReal());
}

TEST(InterpreterTests, EvalSetGlobalVariable)
{
    Interpreter interpreter;
    CheckStackSize(0, interpreter);
    interpreter.PushSymbol("x");
    interpreter.Eval();
    CheckStackSize(1, interpreter);
    EXPECT_TRUE(interpreter.IsNull());
    CheckStackSize(1, interpreter);
    interpreter.Drop();
    CheckStackSize(0, interpreter);
    interpreter.Read("(set x (+ 2 3))");
    interpreter.Eval();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Real, interpreter.Type());
    EXPECT_EQ(5.0, interpreter.PopReal());
    CheckStackSize(0, interpreter);
    interpreter.PushSymbol("x");
    interpreter.Eval();
    CheckStackSize(1, interpreter);
    EXPECT_EQ(ObjType::Real, interpreter.Type());
    EXPECT_EQ(5.0, interpreter.PopReal());
}

}
}
