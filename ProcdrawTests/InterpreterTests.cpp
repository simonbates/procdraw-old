/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Interpreter.h"
#include "TestUtils.h"
#include "ToString.h"

// TODO: Do as many tests as possible with the DocTester
// TODO: Tests for (set var expr)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

// clang-format off

TEST_CLASS(InterpreterTests) {
public:

    TEST_METHOD(EvalSumExpression)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.Read("(+ 1 (+ 2 3))");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(6.0, interpreter.PopReal());
    }

    TEST_METHOD(AssocFindsMatch)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("b");
        AssertStackSize(1, interpreter);
        interpreter.Read("((a . 10) (b . 11) (c . 12))");
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Assoc();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        // TODO: Rather than checking printed value, check is same object with Eq
        //       Read Dup Cadr Swap -- Should put (b . 11) under the alist
        Assert::AreEqual(std::string("(b . 11)"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(AssocNilOnMatchNotFound)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("d");
        AssertStackSize(1, interpreter);
        interpreter.Read("((a . 10) (b . 11) (c . 12))");
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Assoc();
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
    }

    TEST_METHOD(EvalLambdaMakesExpr)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.Read("(lambda (x) (+ x 1))");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Expr, interpreter.Type());
        Assert::AreEqual(std::string("(lambda (x) (+ x 1))"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(EvalLambdaExpr)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.Read("((lambda (x y) (+ (- x y) 1)) 11 5)");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(7.0, interpreter.PopReal());
    }

    TEST_METHOD(EvalSetGlobalVariable)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("x");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
        AssertStackSize(1, interpreter);
        interpreter.Drop();
        AssertStackSize(0, interpreter);
        interpreter.Read("(set x (+ 2 3))");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(5.0, interpreter.PopReal());
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("x");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(5.0, interpreter.PopReal());
    }

};

}
}
