/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Interpreter.h"
#include "TestUtils.h"
#include "ToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

TEST_CLASS(InterpreterReadTests) {
public:

    TEST_METHOD(ReadTrue)
    {
        Interpreter interpreter;
        interpreter.Read("true");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Boolean, interpreter.Type());
        Assert::IsTrue(interpreter.PopBoolean());
    }

    TEST_METHOD(ReadFalse)
    {
        Interpreter interpreter;
        interpreter.Read("false");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Boolean, interpreter.Type());
        Assert::IsFalse(interpreter.PopBoolean());
    }

    TEST_METHOD(ReadEmptyListAsNil)
    {
        Interpreter interpreter;
        interpreter.Read("()");
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
        interpreter.Drop();
        AssertStackSize(0, interpreter);
        interpreter.Read(" ( ) ");
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
    }

    TEST_METHOD(ReadNil)
    {
        Interpreter interpreter;
        interpreter.Read("nil");
        AssertStackSize(1, interpreter);
        Assert::IsTrue(interpreter.IsNull());
    }

    TEST_METHOD(ReadList)
    {
        Interpreter interpreter;
        interpreter.Read("(a b c)");                                 // (a b c)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Next();                                          // (b c) a
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("a"), interpreter.PopSymbol()); // (b c)
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Next();                                          // (c) b
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("b"), interpreter.PopSymbol()); // (c)
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Next();                                          // nil c
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("c"), interpreter.PopSymbol()); // nil
        Assert::IsTrue(interpreter.IsNull());
        AssertStackSize(1, interpreter);
    }

    TEST_METHOD(ReadDottedPair)
    {
        Interpreter interpreter;
        interpreter.Read("(a . b)");                                 // (a . b)
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::ConsPtr, interpreter.Type());
        interpreter.Next();                                          // b a
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("a"), interpreter.PopSymbol()); // b
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("b"), interpreter.PopSymbol());
        AssertStackSize(0, interpreter);
    }

    TEST_METHOD(ReadReal)
    {
        Interpreter interpreter;
        interpreter.Read("42");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::Real, interpreter.Type());
        Assert::AreEqual(42.0, interpreter.PopReal());
    }

    TEST_METHOD(ReadSymbol)
    {
        Interpreter interpreter;
        interpreter.Read("foo");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("foo"), interpreter.PopSymbol());
    }

    TEST_METHOD(ReadPlusSymbol)
    {
        Interpreter interpreter;
        interpreter.Read("+");
        AssertStackSize(1, interpreter);
        Assert::AreEqual(ObjType::SymbolPtr, interpreter.Type());
        Assert::AreEqual(std::string("+"), interpreter.PopSymbol());
    }
};

}
}
