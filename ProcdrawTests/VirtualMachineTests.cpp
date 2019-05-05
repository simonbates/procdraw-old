/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "TestUtils.h"
#include "ToString.h"
#include "VirtualMachine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Procdraw {
namespace Tests {

// clang-format off

TEST_CLASS(VirtualMachineTests) {
public:

    TEST_METHOD(BooleansOnStack)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushBoolean(false);
        AssertStackSize(1, vm);
        vm.PushBoolean(true);
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::Boolean, vm.Type());
        Assert::IsTrue(vm.PopBoolean());
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Boolean, vm.Type());
        Assert::IsFalse(vm.PopBoolean());
        AssertStackSize(0, vm);
    }

    TEST_METHOD(NilOnStack)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushNil();
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Null, vm.Type());
        Assert::IsTrue(vm.IsNull());
    }

    TEST_METHOD(ConsListOfOneElement)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushNil();
        AssertStackSize(1, vm);
        vm.PushSymbol("foo");
        AssertStackSize(2, vm);
        vm.Cons();
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::ConsPtr, vm.Type());
        vm.Dup();
        AssertStackSize(2, vm);
        vm.Car();
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::SymbolPtr, vm.Type());
        Assert::AreEqual(std::string("foo"), vm.PopSymbol());
        AssertStackSize(1, vm);
        vm.Cdr();
        AssertStackSize(1, vm);
        Assert::IsTrue(vm.IsNull());
        vm.Drop();
        AssertStackSize(0, vm);
    }

    TEST_METHOD(ConsListOfTwoElements)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushNil();                  // nil
        AssertStackSize(1, vm);
        vm.PushSymbol("bar");          // nil bar
        AssertStackSize(2, vm);
        vm.Cons();                     // (bar)
        AssertStackSize(1, vm);
        vm.PushSymbol("foo");          // (bar) foo
        AssertStackSize(2, vm);
        vm.Cons();                     // (foo bar)
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::ConsPtr, vm.Type());
        vm.Dup();                      // (foo bar) (foo bar)
        AssertStackSize(2, vm);
        vm.Car();                      // (foo bar) foo
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::SymbolPtr, vm.Type());
        Assert::AreEqual(std::string("foo"), vm.PopSymbol()); // (foo bar)
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::ConsPtr, vm.Type());
        vm.Cdr();                      // (bar)
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::ConsPtr, vm.Type());
        vm.Dup();                      // (bar) (bar)
        AssertStackSize(2, vm);
        vm.Car();                      // (bar) bar
        AssertStackSize(2, vm);
        Assert::AreEqual(std::string("bar"), vm.PopSymbol()); // (bar)
        AssertStackSize(1, vm);
        vm.Cdr();                      // nil
        AssertStackSize(1, vm);
        Assert::IsTrue(vm.IsNull());
        vm.Drop();
        AssertStackSize(0, vm);
    }

    TEST_METHOD(RealsOnStack)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushReal(10);
        AssertStackSize(1, vm);
        vm.PushReal(20);
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(20.0, vm.PopReal());
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(10.0, vm.PopReal());
        AssertStackSize(0, vm);
    }

    TEST_METHOD(SymbolsOnStack)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushSymbol("foo");
        AssertStackSize(1, vm);
        vm.PushSymbol("bar");
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::SymbolPtr, vm.Type());
        Assert::AreEqual(std::string("bar"), vm.PopSymbol());
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::SymbolPtr, vm.Type());
        Assert::AreEqual(std::string("foo"), vm.PopSymbol());
        AssertStackSize(0, vm);
    }

    TEST_METHOD(Add)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushReal(1.25);
        AssertStackSize(1, vm);
        vm.PushReal(2.5);
        AssertStackSize(2, vm);
        vm.Add();
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(3.75, vm.PopReal());
        AssertStackSize(0, vm);
    }

    TEST_METHOD(Dup)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushSymbol("foo");
        AssertStackSize(1, vm);
        vm.Dup();
        AssertStackSize(2, vm);
        vm.PushReal(42.0);
        AssertStackSize(3, vm);
        vm.Dup();
        AssertStackSize(4, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(42.0, vm.PopReal());
        AssertStackSize(3, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(42.0, vm.PopReal());
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::SymbolPtr, vm.Type());
        Assert::AreEqual(std::string("foo"), vm.PopSymbol());
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::SymbolPtr, vm.Type());
        Assert::AreEqual(std::string("foo"), vm.PopSymbol());
        AssertStackSize(0, vm);
    }

    TEST_METHOD(LoadAndStoreGlobalBindings)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushReal(42.0);
        vm.PushSymbol("foo");
        vm.Store();
        AssertStackSize(1, vm);
        Assert::AreEqual(42.0, vm.PopReal());
        AssertStackSize(0, vm);
        vm.PushSymbol("foo");
        vm.Load();
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(42.0, vm.PopReal());
    }

    TEST_METHOD(Next)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushNil();                          // nil
        vm.PushReal(3.0);                      // nil 3
        vm.Cons();                             // (3)
        vm.PushReal(2.0);                      // (3) 2
        vm.Cons();                             // (2 3)
        vm.PushReal(1.0);                      // (2 3) 1
        vm.Cons();                             // (1 2 3)
        AssertStackSize(1, vm);
        vm.Next();                             // (2 3) 1
        AssertStackSize(2, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(1.0, vm.PopReal());   // (2 3)
        vm.Next();                             // (3) 2
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(2.0, vm.PopReal());   // (3)
        vm.Next();                             // nil 3
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(3.0, vm.PopReal());   // nil
        AssertStackSize(1, vm);
        Assert::IsTrue(vm.IsNull());
    }

    TEST_METHOD(Swap)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        vm.PushSymbol("foo");
        AssertStackSize(1, vm);
        vm.PushSymbol("bar");
        AssertStackSize(2, vm);
        vm.Swap();
        AssertStackSize(2, vm);
        Assert::AreEqual(std::string("foo"), vm.PopSymbol());
        AssertStackSize(1, vm);
        Assert::AreEqual(std::string("bar"), vm.PopSymbol());
        AssertStackSize(0, vm);
    }

    TEST_METHOD(LoadLooksUpCurrentEnvBeforeGlobal)
    {
        VirtualMachine vm;
        AssertStackSize(0, vm);
        // Bind global value of foo to 42
        vm.PushReal(42.0);
        vm.PushSymbol("foo");
        vm.Store();
        vm.Drop();
        AssertStackSize(0, vm);
        // Make a new environment and bind foo to 10
        vm.NewEnv();
        vm.PushReal(10.0);
        vm.PushSymbol("foo");
        vm.AddBinding();
        AssertStackSize(0, vm);
        // Look up foo
        vm.PushSymbol("foo");
        vm.Load();
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(10.0, vm.PopReal());
        // Drop the environment and look up foo again
        vm.DropEnv();
        vm.PushSymbol("foo");
        vm.Load();
        AssertStackSize(1, vm);
        Assert::AreEqual(ObjType::Real, vm.Type());
        Assert::AreEqual(42.0, vm.PopReal());
    }

};

}
}
