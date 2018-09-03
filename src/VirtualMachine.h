/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <vector>

// TODO: Rename argsFrameStart_ to framePointer_
// TODO: For ApplyCProcedure, do I need both frame pointer and numArgs?
//       If the stack contains arg0 .. argn proc, then we can calculate
//       one from the other.
// TODO: Another stack layout option for function calls:
//       ( argn .. arg0 proc )
//       And set frame pointer to top of stack (after proc has been popped).
//       Like the cdecl calling convention and more consistent with the
//       existing stack operation argument ordering. For example Cons() is:
//       ( cdr car -- cons )
// TODO: The stack-effect for ApplyCProcedure() is funky:
//       ( arg0 .. argn proc -- arg0 .. argn val )
//       Better to be: ( arg0 .. argn proc -- val )
// TODO: Can I make the frame pointer private and only use for CProcedures?
//       For Lisp lambda expressions, remove the arguments from the stack
//       before we evaluate the expression. Would I ever want to refer to
//       lambda expression arguments by position number?

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

class Interpreter;

typedef void (*CProcedure)(Interpreter* interpreter, int numArgs);

struct Object {
    Object(ObjType type)
        : type(type) {}
    ObjType type;
    union {
        bool boolVal;
        ConsIndexType consIndex;
        ConsIndexType exprIndex;
        CProcedure proc;
        double realVal;
        SymbolIndexType symbolIndex;
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
    // Data types
    void PushBoolean(bool val);
    bool PopBoolean();
    void PushFsubr(CProcedure proc);
    void PushNil();
    void PushReal(double val);
    double PopReal();
    void PushSubr(CProcedure proc);
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
    void ApplyCProcedure(Interpreter* interpreter, int numArgs);
    void Car();
    void Cdr();
    void Cons();
    void DeleteEnv();
    void Drop();
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

protected:
    StackIndexType argsFrameStart_{0};

private:
    std::vector<Object> stack_;
    std::vector<Object> auxStack_;
    std::vector<Object> envStack_;
    std::vector<Symbol> symbolTable_;
    std::vector<ConsCell> listMem_;
    bool Eq(const Object& x, const Object& y);
};

} // namespace Procdraw
