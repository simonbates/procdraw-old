/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "PrintTo.h"
#include "TestUtils.h"
#include "VirtualMachine.h"

namespace Procdraw {
namespace Tests {

TEST(VirtualMachineTests, BooleansOnStack)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushBoolean(false);
    CheckStackSize(1, vm);
    vm.PushBoolean(true);
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::Boolean, vm.Type());
    EXPECT_TRUE(vm.PopBoolean());
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Boolean, vm.Type());
    EXPECT_FALSE(vm.PopBoolean());
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, NilOnStack)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushNil();
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Null, vm.Type());
    EXPECT_TRUE(vm.IsNull());
}

TEST(VirtualMachineTests, ConsListOfOneElement)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushNil();
    CheckStackSize(1, vm);
    vm.PushSymbol("foo");
    CheckStackSize(2, vm);
    vm.Cons();
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::ConsPtr, vm.Type());
    vm.Dup();
    CheckStackSize(2, vm);
    vm.Car();
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::SymbolPtr, vm.Type());
    EXPECT_EQ(std::string("foo"), vm.PopSymbol());
    CheckStackSize(1, vm);
    vm.Cdr();
    CheckStackSize(1, vm);
    EXPECT_TRUE(vm.IsNull());
    vm.Drop();
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, ConsListOfTwoElements)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushNil(); // nil
    CheckStackSize(1, vm);
    vm.PushSymbol("bar"); // nil bar
    CheckStackSize(2, vm);
    vm.Cons(); // (bar)
    CheckStackSize(1, vm);
    vm.PushSymbol("foo"); // (bar) foo
    CheckStackSize(2, vm);
    vm.Cons(); // (foo bar)
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::ConsPtr, vm.Type());
    vm.Dup(); // (foo bar) (foo bar)
    CheckStackSize(2, vm);
    vm.Car(); // (foo bar) foo
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::SymbolPtr, vm.Type());
    EXPECT_EQ(std::string("foo"), vm.PopSymbol()); // (foo bar)
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::ConsPtr, vm.Type());
    vm.Cdr(); // (bar)
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::ConsPtr, vm.Type());
    vm.Dup(); // (bar) (bar)
    CheckStackSize(2, vm);
    vm.Car(); // (bar) bar
    CheckStackSize(2, vm);
    EXPECT_EQ(std::string("bar"), vm.PopSymbol()); // (bar)
    CheckStackSize(1, vm);
    vm.Cdr(); // nil
    CheckStackSize(1, vm);
    EXPECT_TRUE(vm.IsNull());
    vm.Drop();
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, RealsOnStack)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushReal(10);
    CheckStackSize(1, vm);
    vm.PushReal(20);
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(20.0, vm.PopReal());
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(10.0, vm.PopReal());
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, SymbolsOnStack)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushSymbol("foo");
    CheckStackSize(1, vm);
    vm.PushSymbol("bar");
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::SymbolPtr, vm.Type());
    EXPECT_EQ(std::string("bar"), vm.PopSymbol());
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::SymbolPtr, vm.Type());
    EXPECT_EQ(std::string("foo"), vm.PopSymbol());
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, Add)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushReal(1.25);
    CheckStackSize(1, vm);
    vm.PushReal(2.5);
    CheckStackSize(2, vm);
    vm.Add();
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(3.75, vm.PopReal());
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, Dup)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushSymbol("foo");
    CheckStackSize(1, vm);
    vm.Dup();
    CheckStackSize(2, vm);
    vm.PushReal(42.0);
    CheckStackSize(3, vm);
    vm.Dup();
    CheckStackSize(4, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(42.0, vm.PopReal());
    CheckStackSize(3, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(42.0, vm.PopReal());
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::SymbolPtr, vm.Type());
    EXPECT_EQ(std::string("foo"), vm.PopSymbol());
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::SymbolPtr, vm.Type());
    EXPECT_EQ(std::string("foo"), vm.PopSymbol());
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, LoadAndStoreGlobalBindings)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushReal(42.0);
    vm.PushSymbol("foo");
    vm.Store();
    CheckStackSize(1, vm);
    EXPECT_EQ(42.0, vm.PopReal());
    CheckStackSize(0, vm);
    vm.PushSymbol("foo");
    vm.Load();
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(42.0, vm.PopReal());
}

TEST(VirtualMachineTests, Next)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushNil();     // nil
    vm.PushReal(3.0); // nil 3
    vm.Cons();        // (3)
    vm.PushReal(2.0); // (3) 2
    vm.Cons();        // (2 3)
    vm.PushReal(1.0); // (2 3) 1
    vm.Cons();        // (1 2 3)
    CheckStackSize(1, vm);
    vm.Next(); // (2 3) 1
    CheckStackSize(2, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(1.0, vm.PopReal()); // (2 3)
    vm.Next();                    // (3) 2
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(2.0, vm.PopReal()); // (3)
    vm.Next();                    // nil 3
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(3.0, vm.PopReal()); // nil
    CheckStackSize(1, vm);
    EXPECT_TRUE(vm.IsNull());
}

TEST(VirtualMachineTests, Swap)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    vm.PushSymbol("foo");
    CheckStackSize(1, vm);
    vm.PushSymbol("bar");
    CheckStackSize(2, vm);
    vm.Swap();
    CheckStackSize(2, vm);
    EXPECT_EQ(std::string("foo"), vm.PopSymbol());
    CheckStackSize(1, vm);
    EXPECT_EQ(std::string("bar"), vm.PopSymbol());
    CheckStackSize(0, vm);
}

TEST(VirtualMachineTests, LoadLooksUpCurrentEnvBeforeGlobal)
{
    VirtualMachine vm;
    CheckStackSize(0, vm);
    // Bind global value of foo to 42
    vm.PushReal(42.0);
    vm.PushSymbol("foo");
    vm.Store();
    vm.Drop();
    CheckStackSize(0, vm);
    // Make a new environment and bind foo to 10
    vm.NewEnv();
    vm.PushReal(10.0);
    vm.PushSymbol("foo");
    vm.AddBinding();
    CheckStackSize(0, vm);
    // Look up foo
    vm.PushSymbol("foo");
    vm.Load();
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(10.0, vm.PopReal());
    // Drop the environment and look up foo again
    vm.DropEnv();
    vm.PushSymbol("foo");
    vm.Load();
    CheckStackSize(1, vm);
    EXPECT_EQ(ObjType::Real, vm.Type());
    EXPECT_EQ(42.0, vm.PopReal());
}

} // namespace Tests
} // namespace Procdraw
