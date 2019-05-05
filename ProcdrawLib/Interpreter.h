/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "Printer.h"
#include "Reader.h"
#include "VirtualMachine.h"
#include <string>
#include <vector>

// TODO: Figure out error checking and communication
//       - C++ Exceptions?
//       - Check stack has enough elements
//       - Check object types
//       - std::vector bounds checking:
//           - at()
//       - std::vector with no bounds checking:
//           - operator[]
//           - back()
//           - pop_back()
// TODO: Testing proposal:
//       - For programming API, use narrow contracts plus assert()s
//           - Like std::vector::operator[]
//           - Drop(), Cons(), and such
//           - Calling outside preconditions is undefined behaviour
//           - Such as empty stack
//           - Do defensive programming with assert()s, which will check in debug mode
//           - It is the client's responsibility to call within contract
//       - For code processing user input, validate and throw exceptions
//           - Read()
//           - Eval()
//           - CProcedure implementations: check number and types of arguments

// TODO: Reference counting for Lists
// TODO: Make a class VirtualMachineInspector friend of VirtualMachine that can
//       be used in tests to look at internals (such as reference counts)
// TODO: Environments: Set of (symbol, value) bindings together with pointer to
//       parent environment. Maybe an environmentStack to keep track of
//       active environment? Then Get and Set would be in the context of the
//       environment on the top of the environmentStack? To implement closures,
//       I think I will need a Function object type, created when a function
//       definition is evaluated that captures the environment that was active
//       at that time. When a Function is called, it uses the environment that
//       was active at definition (extended with arguments) not the environment
//       active at call time. Each Function call (or let) establishes a new
//       environment (with the function args) and is pushed onto the
//       environmentStack. When a Function call completes, the environment
//       is popped of the stack.
// TODO: Maybe use an API more like Lua's?
//       - Access objects on the stack by index
//           - Relative to stack frame (+ve), and relative to top of stack (-ve)
// TODO: Make Interpreters first class objects within the interpreter
//       (clone-interpreter) creates a new Interpreter that is a clone of the current one
//           Calls InterpreterSwitcher::CloneInterpreter(this)
//       (use-interpreter interpreter) sets the active interpreter
//           Calls InterpreterSwitcher::UseInterpreter()
//       parent-interpreter points to the interpreter that this interpreter was cloned from
// TODO: Need a solution for iterating through lists
//       while (!Null()) {
//           Next();
//           ...
//       }
//       Is not safe because the list might not terminate in nil and Next() is
//       not specified for non-ConsPtr types.
// TODO: Have Next() throw an exception if not ConsPtr (to handle case of
//       iterating through an improper list, when we expect a proper list)
// TODO: Solution for managing Lisp stack unwind when C++ exception thrown
//       (and Lisp memory management more generally in presence of C++
//       exceptions)

namespace Procdraw {

class Interpreter {
public:
    Interpreter();
    void Add() { vm_.Add(); }
    void Assoc();
    void Call(int numArgs);
    void Cons() { vm_.Cons(); }
    void Drop() { vm_.Drop(); }
    void Dup() { vm_.Dup(); }
    void ExprBody() { vm_.ExprBody(); }
    void ExprParams() { vm_.ExprParams(); }
    void Eval();
    bool IsAtom() { return vm_.IsAtom(); }
    bool IsNull() { return vm_.IsNull(); }
    void MakeExpr() { vm_.MakeExpr(); }
    void Mul() { vm_.Mul(); }
    void Next() { vm_.Next(); }
    bool PopBoolean() { return vm_.PopBoolean(); }
    double PopReal() { return vm_.PopReal(); }
    std::string PopSymbol() { return vm_.PopSymbol(); }
    std::string PrintToString();
    void PushArg(StackIndexType n) { vm_.PushArg(n); }
    void PushBoolean(bool val) { vm_.PushBoolean(val); }
    void PushNil() { vm_.PushNil(); }
    void PushReal(double val) { vm_.PushReal(val); }
    void PushSymbol(const std::string& name) { vm_.PushSymbol(name); }
    void Read(const std::string& str);
    StackIndexType StackSize() const { return vm_.StackSize(); }
    void Store() { vm_.Store(); }
    void Sub() { vm_.Sub(); }
    void Swap() { vm_.Swap(); }
    ObjType Type() { return vm_.Type(); }

private:
    VirtualMachine vm_;
    Printer printer_;
    void CallExpr(int numArgs);
    void EvalProcedureCall();
    int ListElems(bool evalElems);
    void StoreFsubr(const std::string& var, SysFunc sysFunc);
    void StoreSubr(const std::string& var, SysFunc sysFunc);
};

} // namespace Procdraw
