#include "stdafx.h"
#include "Interpreter.h"
#include "ToString.h"

// TODO: Do as many tests as possible with the DocTester
// TODO: Tests for (set var expr)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace procdraw {
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
        interpreter.PushNil();                          // nil
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("bar");                  // nil bar
        AssertStackSize(2, interpreter);
        interpreter.Cons();                             // (bar)
        AssertStackSize(1, interpreter);
        interpreter.PushSymbol("foo");                  // (bar) foo
        AssertStackSize(2, interpreter);
        interpreter.Cons();                             // (foo bar)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Dup();                              // (foo bar) (foo bar)
        AssertStackSize(2, interpreter);
        interpreter.Car();                              // (foo bar) foo
        AssertStackSize(2, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol()); // (foo bar)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Cdr();                              // (bar)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Dup();                              // (bar) (bar)
        AssertStackSize(2, interpreter);
        interpreter.Car();                              // (bar) bar
        AssertStackSize(2, interpreter);
        Assert::AreEqual(std::string("bar"), interpreter.PopSymbol()); // (bar)
        AssertStackSize(1, interpreter);
        interpreter.Cdr();                              // nil
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

};

}
}

/*

TEST_METHOD("Interpreter stack should support symbols")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("foo");
    AssertStackSize(, interpreter1);
    interpreter.PushSymbol("bar");
    AssertStackSize(, interpreter2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "bar");
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    AssertStackSize(, interpreter0);
}

TEST_METHOD("Add should add 2 reals")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushReal(1.25);
    AssertStackSize(, interpreter1);
    interpreter.PushReal(2.5);
    AssertStackSize(, interpreter2);
    interpreter.Add();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 3.75);
    AssertStackSize(, interpreter0);
}

TEST_METHOD("Dup should duplicate top of stack")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("foo");
    AssertStackSize(, interpreter1);
    interpreter.Dup();
    AssertStackSize(, interpreter2);
    interpreter.PushReal(42.0);
    AssertStackSize(, interpreter3);
    interpreter.Dup();
    AssertStackSize(, interpreter4);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
    AssertStackSize(, interpreter3);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
    AssertStackSize(, interpreter2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    AssertStackSize(, interpreter0);
}

TEST_METHOD("Eval (+ 1 (+ 2 3)) should be 6")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.Read("(+ 1 (+ 2 3))");
    interpreter.Eval();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 6.0);
}

TEST_METHOD("Load and Store should provide global bindings")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushReal(42.0);
    interpreter.PushSymbol("foo");
    interpreter.Store();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("foo");
    interpreter.Load();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
}

TEST_METHOD("Next should iterate through a list structure")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushNil();                              // nil
    interpreter.PushReal(3.0);                          // nil 3
    interpreter.Cons();                                 // (3)
    interpreter.PushReal(2.0);                          // (3) 2
    interpreter.Cons();                                 // (2 3)
    interpreter.PushReal(1.0);                          // (2 3) 1
    interpreter.Cons();                                 // (1 2 3)
    AssertStackSize(, interpreter1);
    interpreter.Next();                                 // (2 3) 1
    AssertStackSize(, interpreter2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 1.0);              // (2 3)
    interpreter.Next();                                 // (3) 2
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 2.0);              // (3)
    interpreter.Next();                                 // nil 3
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 3.0);              // nil
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.IsNull());
}

TEST_METHOD("Swap should swap the top 2 elements on the stack")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("foo");
    AssertStackSize(, interpreter1);
    interpreter.PushSymbol("bar");
    AssertStackSize(, interpreter2);
    interpreter.Swap();
    AssertStackSize(, interpreter2);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.PopSymbol() == "bar");
    AssertStackSize(, interpreter0);
}

TEST_METHOD("Assoc(((a . 10) (b . 11) (c . 12)), b) should be (b . 11)")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("b");
    AssertStackSize(, interpreter1);
    interpreter.Read("((a . 10) (b . 11) (c . 12))");
    AssertStackSize(, interpreter2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    interpreter.Assoc();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    // TODO: Rather than checking printed value, check is same object with Eq
    //       Read Dup Cadr Swap -- Should put (b . 11) under the alist
    Assert::AreEqual(interpreter.PrintToString() == "(b . 11)");
    AssertStackSize(, interpreter0);
}

TEST_METHOD("Assoc(((a . 10) (b . 11) (c . 12)), d) should be nil")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("d");
    AssertStackSize(, interpreter1);
    interpreter.Read("((a . 10) (b . 11) (c . 12))");
    AssertStackSize(, interpreter2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    interpreter.Assoc();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.IsNull());
}

TEST_METHOD("Evaluating a lambda should make an Expr")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.Read("(lambda (x) (+ x 1))");
    interpreter.Eval();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Expr);
    Assert::AreEqual(interpreter.PrintToString() == "(lambda (x) (+ x 1))");
    AssertStackSize(, interpreter0);
}

TEST_METHOD("Load should look up current environment and then global")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    // Bind global value of foo to 42
    interpreter.PushReal(42.0);
    interpreter.PushSymbol("foo");
    interpreter.Store();
    interpreter.Drop();
    AssertStackSize(, interpreter0);
    // Make a new environment and bind foo to 10
    interpreter.NewEnv();
    interpreter.PushReal(10.0);
    interpreter.PushSymbol("foo");
    interpreter.AddBinding();
    AssertStackSize(, interpreter0);
    // Look up foo
    interpreter.PushSymbol("foo");
    interpreter.Load();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 10.0);
    // Delete environment and look up foo again
    interpreter.DeleteEnv();
    interpreter.PushSymbol("foo");
    interpreter.Load();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
}

TEST_METHOD("Eval ((lambda (x) (+ x 1)) 2) should be 3")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.Read("((lambda (x) (+ x 1)) 2)");
    interpreter.Eval();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 3);
}

TEST_METHOD("Eval (set x (+ 2 3)) should set global variable x to 5")
{
    procdraw::Interpreter interpreter;
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("x");
    interpreter.Eval();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.IsNull());
    AssertStackSize(, interpreter1);
    interpreter.Drop();
    AssertStackSize(, interpreter0);
    interpreter.Read("(set x (+ 2 3))");
    interpreter.Eval();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 5);
    AssertStackSize(, interpreter0);
    interpreter.PushSymbol("x");
    interpreter.Eval();
    AssertStackSize(, interpreter1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 5);
}

*/
