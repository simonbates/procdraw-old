/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Interpreter.h"
#include "ToString.h"

// TODO: Do as many tests as possible with the DocTester
// TODO: Tests for (set var expr)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

void AssertStackSize(StackIndexType expected, const Interpreter& interpreter)
{
    Assert::AreEqual(expected, interpreter.StackSize());
}

TEST_CLASS(InterpreterTests) {
public:

    TEST_METHOD(BooleansOnStack)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushBoolean(false);
        AssertStackSize(1, interpreter);
        interpreter.PushBoolean(true);
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::Boolean, interpreter.Type());
        Assert::IsTrue(interpreter.PopBoolean());
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Boolean, interpreter.Type());
        Assert::IsFalse(interpreter.PopBoolean());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(NilOnStack)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushNil();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Null, interpreter.Type());
        Assert::IsTrue(interpreter.IsNull());
    }

    TEST_METHOD(ConsListOfOneElement)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushNil();
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("foo");
        AssertStackSize(2, interpreter);
        interpreter.Cons();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Dup();
        AssertStackSize(2, interpreter);
        interpreter.Car();
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol());
        AssertStackSize(1, interpreter);
        interpreter.Cdr();
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
        interpreter.Drop();
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(ConsListOfTwoElements)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushNil();                  // nil
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("bar");          // nil bar
        AssertStackSize(2, interpreter);
        interpreter.Cons();                     // (bar)
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("foo");          // (bar) foo
        AssertStackSize(2, interpreter);
        interpreter.Cons();                     // (foo bar)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Dup();                      // (foo bar) (foo bar)
        AssertStackSize(2, interpreter);
        interpreter.Car();                      // (foo bar) foo
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol()); // (foo bar)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Cdr();                      // (bar)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Dup();                      // (bar) (bar)
        AssertStackSize(2, interpreter);
        interpreter.Car();                      // (bar) bar
        AssertStackSize(2, interpreter);
        Assert::AreEqual(std::string("bar"), interpreter.PopSymbol()); // (bar)
        AssertStackSize(1, interpreter);
        interpreter.Cdr();                      // nil
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
        interpreter.Drop();
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(RealsOnStack)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushReal(10);
        AssertStackSize(1, interpreter);
        interpreter.PushReal(20);
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(20.0, interpreter.PopReal());
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(10.0, interpreter.PopReal());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(SymbolsOnStack)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("foo");
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("bar");
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("bar"), interpreter.PopSymbol());
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(Add)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushReal(1.25);
        AssertStackSize(1, interpreter);
        interpreter.PushReal(2.5);
        AssertStackSize(2, interpreter);
        interpreter.Add();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(3.75, interpreter.PopReal());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(Dup)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("foo");
        AssertStackSize(1, interpreter);
        interpreter.Dup();
        AssertStackSize(2, interpreter);
        interpreter.PushReal(42.0);
        AssertStackSize(3, interpreter);
        interpreter.Dup();
        AssertStackSize(4, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(42.0, interpreter.PopReal());
        AssertStackSize(3, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(42.0, interpreter.PopReal());
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol());
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol());
        AssertStackSize(0, interpreter);
    }

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

    TEST_METHOD(LoadAndStoreGlobalBindings)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushReal(42.0);
        interpreter.PushSymbol("foo");
        interpreter.Store();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(42.0, interpreter.PopReal());
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("foo");
        interpreter.Load();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(42.0, interpreter.PopReal());
    }

    TEST_METHOD(Next)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushNil();                          // nil
        interpreter.PushReal(3.0);                      // nil 3
        interpreter.Cons();                             // (3)
        interpreter.PushReal(2.0);                      // (3) 2
        interpreter.Cons();                             // (2 3)
        interpreter.PushReal(1.0);                      // (2 3) 1
        interpreter.Cons();                             // (1 2 3)
        AssertStackSize(1, interpreter);
        interpreter.Next();                             // (2 3) 1
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(1.0, interpreter.PopReal());   // (2 3)
        interpreter.Next();                             // (3) 2
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(2.0, interpreter.PopReal());   // (3)
        interpreter.Next();                             // nil 3
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(3.0, interpreter.PopReal());   // nil
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
    }

    TEST_METHOD(Swap)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.PushSymbol("foo");
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("bar");
        AssertStackSize(2, interpreter);
        interpreter.Swap();
        AssertStackSize(2, interpreter);
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol());
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("bar"), interpreter.PopSymbol());
        AssertStackSize(0, interpreter);
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

    TEST_METHOD(LoadLooksUpCurrentEnvBeforeGlobal)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        // Bind global value of foo to 42
        interpreter.PushReal(42.0);
        interpreter.PushSymbol("foo");
        interpreter.Store();
        interpreter.Drop();
        AssertStackSize(0, interpreter);
        // Make a new environment and bind foo to 10
        interpreter.NewEnv();
        interpreter.PushReal(10.0);
        interpreter.PushSymbol("foo");
        interpreter.AddBinding();
        AssertStackSize(0, interpreter);
        // Look up foo
        interpreter.PushSymbol("foo");
        interpreter.Load();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(10.0, interpreter.PopReal());
        // Delete environment and look up foo again
        interpreter.DeleteEnv();
        interpreter.PushSymbol("foo");
        interpreter.Load();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(42.0, interpreter.PopReal());
    }

    TEST_METHOD(EvalExpr)
    {
        Interpreter interpreter;
        AssertStackSize(0, interpreter);
        interpreter.Read("((lambda (x) (+ x 1)) 2)");
        interpreter.Eval();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(3.0, interpreter.PopReal());
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
