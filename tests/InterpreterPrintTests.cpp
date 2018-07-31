/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Interpreter.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

TEST_CLASS(InterpreterPrintTests) {
public:

    TEST_METHOD(PrintToStringAtoms)
    {
        Interpreter interpreter;

        interpreter.PushReal(42);
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("42"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);

        interpreter.PushBoolean(true);
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("true"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);

        interpreter.PushBoolean(false);
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("false"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);

        interpreter.PushSymbol("foo");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("foo"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(PrintToStringNil)
    {
        Interpreter interpreter;
        interpreter.PushNil();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("nil"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(PrintToStringLists)
    {
        Interpreter interpreter;

        interpreter.Read("(a b c)");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("(a b c)"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);

        interpreter.Read("(lambda (param1 param2) (call1) (call2 arg1 arg2 arg3))");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("(lambda (param1 param2) (call1) (call2 arg1 arg2 arg3))"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(PrintToStringDottedPair)
    {
        Interpreter interpreter;
        interpreter.PushSymbol("b");
        interpreter.PushSymbol("a");
        interpreter.Cons();
        AssertStackSize(1, interpreter);
        Assert::AreEqual(std::string("(a . b)"), interpreter.PrintToString());
        AssertStackSize(0, interpreter);
    }

};

}
}
