/* Copyright (c) 2018 Simon Bates
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
// TODO: Add numArgs to Apply(): Apply(numArgs)
// TODO: Maybe use an API more like Lua's?
//       - Access objects on the stack by index
//           - Relative to stack frame (+ve), and relative to top of stack (-ve)
//       - Automatic clean-up of stack at end of function call
// TODO: Isolate memory operations from interpreter and access through stack API
//       - Focuses places where checking of stack size, object type,
//         and reference count updating need to be done
//       - And more easily enables changing of memory/stack implementation
//       Some options:
//       - Make a VirtualMachine class a member of Interpreter; wrap operations
//         in inline methods on Interpreter that forward onto member; such as:
//             public:
//                 ...
//                 void Dup() { state.Dup() }
//         This would allow multiple states per Interpreter
//       - Make Interpreter a subclass of VirtualMachine
//         One state per interpreter; something above would hold multiple
//         interpreters (rather than one interpreter with multiple states)
// TODO: Make Interpreters first class objects within the interpreter
//       (clone-interpreter) creates a new Interpreter that is a clone of the current one
//           Calls InterpreterSwitcher::CloneInterpreter(this)
//       (use-interpreter interpreter) sets the active interpreter
//           Calls InterpreterSwitcher::UseInterpreter()
//       parent-interpreter points to the interpreter that this interpreter was cloned from
// TODO: New policy:
//     - All Interpreter methods may access the stack vector directly
//     - The stack vector encapsulation boundary is the Interpreter
//     - Take care: if I have a C++ variable of a ConsPtr, it's possible that
//       an operation could invalidate the pointer between aquiring the pointer
//       and using it. For example, the cons could have been garbage collected
//       due to the reference count being reduced to 0.
// TODO: Need a solution for iterating through lists
//       while (!Null()) {
//           Next();
//           ...
//       }
//       Is not safe because the list might not terminate in nil and Next() is
//       not specified for non-ConsPtr types.
// TODO: Have Next() throw an exception if not ConsPtr (to handle case of
//       iterating through an improper list, when we expect a proper list)
// TODO: Do Cons reference count increment and decrement in Object construction
//       and destruction -- a smart pointer. This will support convenient use
//       of Object outside of the stack(s).
// TODO: Solution for managing Lisp stack unwind when C++ exception thrown
//       (and Lisp memory management more generally in presence of C++
//       exceptions)

namespace Procdraw {

class Interpreter : public VirtualMachine {
public:
    Interpreter();
    void Apply(int numArgs);
    void Assoc();
    void Eval();
    std::string PrintToString();
    void Read(const std::string& str);

private:
    Printer printer_;
    void ApplyExpr(int numArgs);
    void EvalProcedureCall();
    int ListElems(bool evalElems);
    void StoreFsubr(const std::string& var, CProcedure proc);
    void StoreSubr(const std::string& var, CProcedure proc);
};

} // namespace Procdraw
