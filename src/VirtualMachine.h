/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <vector>

// TODO: Another stack layout option for function calls:
//       ( argn .. arg0 proc )
//       And set frame pointer to top of stack (after proc has been popped).
//       Like the cdecl calling convention and more consistent with the
//       existing stack operation argument ordering. For example Cons() is:
//       ( cdr car -- cons )
// TODO: Can I make the frame pointer private and only use for SysFuncs?
//       For Lisp lambda expressions, remove the arguments from the stack
//       before we evaluate the expression. Would I ever want to refer to
//       lambda expression arguments by position number?
// TODO: How about taking the mathematical operators out of VirtualMachine?
//       And implementing them in the Interpreter primitive functions
//       (*, +, -, /)
//       And then its focus would be: Memory
//           - Heap and access to the Heap via a stack
//           - Symbol table
//           - Function call arguments
//           - Environments for Lisp function intrepretation
//       I would need a method for retrieving Real values from the arguments
//       part of the stack (not just the top).
//       Maybe a mechanism similar to Lua where operations take a stack
//       index? https://www.lua.org/manual/5.1/manual.html#3.1
//       Could I then get rid of the environment stack and just use the
//       regular stack? If there's a convenient way to get at parts of the
//       stack frame, I can just put it at the start of the stack frame?

namespace Procdraw {

enum class ObjType {
    Boolean,
    ConsPtr,
    Expr,
    Fsubr,
    Null,
    Real,
    Subr,
    SymbolPtr
};

using StackIndexType = size_t;
using SymbolIndexType = size_t;
using ConsIndexType = size_t;

// TODO: Should VirtualMachine really know about Interpreter?
// TODO: Should I just merge Interpreter and VirtualMachine?
// Motivation for making them separate: to minimise access to internal
// data structures. If objects are retrieved directly and references
// stored in C++ objects, there is risk that reference counting is off
// and we end up with dangling references. I want the VM state to be
// self-contained so that I can make copies and process it. The stack
// API should be used as much as possible.
// Could I maintain the inheritance structure but make changes to make
// the relationship clearer?
// - Make this class abstract, with an unimplemented Call() method as
//   is likely the only one SysFuncs would need access to and then use
//   that in the SysFunc signature
// - Or, use a new purely abstract class (interface) in the SysFunc
//   signature that Interpreter implements
// - Option:
//     - Interpreter interface
//     - InterpreterBase this class
//     - InterpreterImpl current Interpreter
//  - Or:
//     - Interpreter interface
//     - VirtulaMachine class (or rename to InterpreterState,
//       or InterpreterVirtualMachine)
//     - InterpreterImpl implements Interpreter, contains VirtualMachine
//       and uses forwarding methods to vm
// Make forwarding methods inline so that they can be optimized
class Interpreter;

typedef void (*SysFunc)(int numArgs, Interpreter* interpreter);

struct Object {
    Object(ObjType type)
        : type(type) {}
    ObjType type;
    union {
        bool boolVal;
        ConsIndexType consIndex;
        ConsIndexType exprIndex;
        double realVal;
        SymbolIndexType symbolIndex;
        SysFunc sysFunc;
    } val;
};

struct Symbol {
    Symbol(const std::string& name)
        : name(name), value(ObjType::Null) {}
    std::string name;
    Object value;
};

struct ConsCell {
    ConsCell(Object car, Object cdr)
        : car(car), cdr(cdr) {}
    Object car;
    Object cdr;
};

class VirtualMachine {
public:
    StackIndexType framePointer_{0};

    // Data types
    void PushBoolean(bool val);
    bool PopBoolean();
    void PushFsubr(SysFunc sysFunc);
    void PushNil();
    void PushReal(double val);
    double PopReal();
    void PushSubr(SysFunc sysFunc);
    void PushSymbol(const std::string& name);
    std::string PopSymbol();
    // Query the stack
    bool IsAtom();
    bool IsNull();
    StackIndexType StackSize() const;
    ObjType Type();
    // Operations
    void Add();
    void AddBinding();
    void CallSysFunc(int numArgs, Interpreter* interpreter);
    void Car();
    void Cdr();
    void Cons();
    void Drop();
    void DropEnv();
    void Dup();
    bool Eq();
    bool EqCar(StackIndexType index, StackIndexType consIndex);
    void ExprBody();
    void ExprParams();
    void FromAux();
    void Load();
    void MakeExpr();
    void Mul();
    void Next();
    void NewEnv();
    void Nip();
    void Pick(StackIndexType n);
    void PushArg(StackIndexType n);
    void Store();
    void Sub();
    void Swap();
    void ToAux();

private:
    std::vector<Object> stack_;
    std::vector<Object> auxStack_;
    std::vector<Object> envStack_;
    std::vector<Symbol> symbolTable_;
    std::vector<ConsCell> listMem_;
    bool Eq(const Object& x, const Object& y);
};

} // namespace Procdraw
