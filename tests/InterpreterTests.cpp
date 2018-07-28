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

};

}
}

/*

TEST_METHOD(NilOnStack)
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushNil();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Null);
    Assert::AreEqual(interpreter.IsNull());
}

TEST_METHOD("Cons(foo, nil) should make a list of one element")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushNil();
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushSymbol("foo");
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Cons();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    Assert::AreEqual_FALSE(interpreter.IsNull());
    interpreter.Dup();
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Car();
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Cdr();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.IsNull());
    interpreter.Drop();
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Cons(foo, Cons(bar, nil)) should make a list of two elements")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushNil();                        // nil
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushSymbol("bar");                      // nil bar
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Cons();                                 // (bar)
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushSymbol("foo");                      // (bar) foo
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Cons();                                 // (foo bar)
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    Assert::AreEqual_FALSE(interpreter.IsNull());
    interpreter.Dup();                                  // (foo bar) (foo bar)
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Car();                                 // (foo bar) foo
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");          // (foo bar)
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    Assert::AreEqual_FALSE(interpreter.IsNull());
    interpreter.Cdr();                                 // (bar)
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    Assert::AreEqual_FALSE(interpreter.IsNull());
    interpreter.Dup();                                  // (bar) (bar)
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Car();                                 // (bar) bar
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.PopSymbol() == "bar");          // (bar)
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Cdr();                                 // nil
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.IsNull());
    interpreter.Drop();
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Interpreter stack should support reals")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushReal(10);
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushReal(20);
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 20);
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 10);
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Interpreter stack should support symbols")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("foo");
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushSymbol("bar");
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "bar");
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Add should add 2 reals")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushReal(1.25);
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushReal(2.5);
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Add();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 3.75);
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Dup should duplicate top of stack")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("foo");
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Dup();
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.PushReal(42.0);
    Assert::AreEqual(interpreter.StackSize() == 3);
    interpreter.Dup();
    Assert::AreEqual(interpreter.StackSize() == 4);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
    Assert::AreEqual(interpreter.StackSize() == 3);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::SymbolPtr);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Eval (+ 1 (+ 2 3)) should be 6")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.Read("(+ 1 (+ 2 3))");
    interpreter.Eval();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 6.0);
}

TEST_METHOD("Load and Store should provide global bindings")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushReal(42.0);
    interpreter.PushSymbol("foo");
    interpreter.Store();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("foo");
    interpreter.Load();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
}

TEST_METHOD("Next should iterate through a list structure")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushNil();                              // nil
    interpreter.PushReal(3.0);                          // nil 3
    interpreter.Cons();                                 // (3)
    interpreter.PushReal(2.0);                          // (3) 2
    interpreter.Cons();                                 // (2 3)
    interpreter.PushReal(1.0);                          // (2 3) 1
    interpreter.Cons();                                 // (1 2 3)
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Next();                                 // (2 3) 1
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 1.0);              // (2 3)
    interpreter.Next();                                 // (3) 2
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 2.0);              // (3)
    interpreter.Next();                                 // nil 3
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 3.0);              // nil
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.IsNull());
}

TEST_METHOD("Swap should swap the top 2 elements on the stack")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("foo");
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.PushSymbol("bar");
    Assert::AreEqual(interpreter.StackSize() == 2);
    interpreter.Swap();
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.PopSymbol() == "foo");
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.PopSymbol() == "bar");
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Assoc(((a . 10) (b . 11) (c . 12)), b) should be (b . 11)")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("b");
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Read("((a . 10) (b . 11) (c . 12))");
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    interpreter.Assoc();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    // TODO: Rather than checking printed value, check is same object with Eq
    //       Read Dup Cadr Swap -- Should put (b . 11) under the alist
    Assert::AreEqual(interpreter.PrintToString() == "(b . 11)");
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Assoc(((a . 10) (b . 11) (c . 12)), d) should be nil")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("d");
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Read("((a . 10) (b . 11) (c . 12))");
    Assert::AreEqual(interpreter.StackSize() == 2);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::ConsPtr);
    interpreter.Assoc();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.IsNull());
}

TEST_METHOD("Evaluating a lambda should make an Expr")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.Read("(lambda (x) (+ x 1))");
    interpreter.Eval();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Expr);
    Assert::AreEqual(interpreter.PrintToString() == "(lambda (x) (+ x 1))");
    Assert::AreEqual(interpreter.StackSize() == 0);
}

TEST_METHOD("Load should look up current environment and then global")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    // Bind global value of foo to 42
    interpreter.PushReal(42.0);
    interpreter.PushSymbol("foo");
    interpreter.Store();
    interpreter.Drop();
    Assert::AreEqual(interpreter.StackSize() == 0);
    // Make a new environment and bind foo to 10
    interpreter.NewEnv();
    interpreter.PushReal(10.0);
    interpreter.PushSymbol("foo");
    interpreter.AddBinding();
    Assert::AreEqual(interpreter.StackSize() == 0);
    // Look up foo
    interpreter.PushSymbol("foo");
    interpreter.Load();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 10.0);
    // Delete environment and look up foo again
    interpreter.DeleteEnv();
    interpreter.PushSymbol("foo");
    interpreter.Load();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 42.0);
}

TEST_METHOD("Eval ((lambda (x) (+ x 1)) 2) should be 3")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.Read("((lambda (x) (+ x 1)) 2)");
    interpreter.Eval();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 3);
}

TEST_METHOD("Eval (set x (+ 2 3)) should set global variable x to 5")
{
    procdraw::Interpreter interpreter;
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("x");
    interpreter.Eval();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.IsNull());
    Assert::AreEqual(interpreter.StackSize() == 1);
    interpreter.Drop();
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.Read("(set x (+ 2 3))");
    interpreter.Eval();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 5);
    Assert::AreEqual(interpreter.StackSize() == 0);
    interpreter.PushSymbol("x");
    interpreter.Eval();
    Assert::AreEqual(interpreter.StackSize() == 1);
    Assert::AreEqual(interpreter.Type() == procdraw::ObjType::Real);
    Assert::AreEqual(interpreter.PopReal() == 5);
}

*/
