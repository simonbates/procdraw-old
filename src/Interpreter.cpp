/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Interpreter.h"
#include <cassert>
#include <utility>

namespace Procdraw {

void FsubrLambda(Interpreter* interpreter, int numArgs)
{
    // TODO: Support lambda expressions with more than one body form
    interpreter->PushArg(1); // body
    interpreter->PushArg(0); // args
    interpreter->MakeExpr();
}

void FsubrSet(Interpreter* interpreter, int numArgs)
{
    // TODO: Check number and types of arguments
    interpreter->PushArg(1); // expr
    interpreter->Eval();
    interpreter->PushArg(0); // var
    interpreter->Store();
}

void SubrProduct(Interpreter* interpreter, int numArgs)
{
    // TODO: Check arguments are all Reals
    interpreter->PushReal(1.0);
    for (int i = 0; i < numArgs; ++i) {
        interpreter->PushArg(i);
        interpreter->Mul();
    }
}

void SubrSum(Interpreter* interpreter, int numArgs)
{
    // TODO: Check arguments are all Reals
    interpreter->PushReal(0.0);
    for (int i = 0; i < numArgs; ++i) {
        interpreter->PushArg(i);
        interpreter->Add();
    }
}

Interpreter::Interpreter()
{
    StoreFsubr("lambda", FsubrLambda);
    StoreFsubr("set", FsubrSet);
    StoreSubr("*", SubrProduct);
    StoreSubr("+", SubrSum);
}

// ( -- boolean )
void Interpreter::PushBoolean(bool val)
{
    Object obj(ObjType::Boolean);
    obj.val.boolVal = val;
    stack_.push_back(obj);
}

// ( x -- )
bool Interpreter::PopBoolean()
{
    assert(!stack_.empty());
    // TODO: Convert to boolean
    bool val = stack_.back().val.boolVal;
    stack_.pop_back();
    return val;
}

// ( -- fsubr )
void Interpreter::PushFsubr(CProcedure proc)
{
    Object obj(ObjType::Fsubr);
    obj.val.proc = proc;
    stack_.push_back(obj);
}

// ( -- nil )
void Interpreter::PushNil()
{
    stack_.emplace_back(ObjType::Null);
}

// ( -- real )
void Interpreter::PushReal(double val)
{
    Object obj(ObjType::Real);
    obj.val.realVal = val;
    stack_.push_back(obj);
}

// ( real -- )
// TOS must be a Real
double Interpreter::PopReal()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::Real);
    double val = stack_.back().val.realVal;
    stack_.pop_back();
    return val;
}

// ( -- subr )
void Interpreter::PushSubr(CProcedure proc)
{
    Object obj(ObjType::Subr);
    obj.val.proc = proc;
    stack_.push_back(obj);
}

// ( -- symbol )
void Interpreter::PushSymbol(const std::string& name)
{
    // Look for the symbol
    SymbolIndexType i = 0;
    while (i < symbolTable_.size()) {
        if (symbolTable_[i].name == name) {
            break;
        }
        ++i;
    }

    // If not found, add it
    if (i == symbolTable_.size()) {
        symbolTable_.emplace_back(name);
    }

    Object obj(ObjType::SymbolPtr);
    obj.val.symbolIndex = i;
    stack_.push_back(obj);
}

// ( symbol -- )
// TOS must be a SymbolPtr
std::string Interpreter::PopSymbol()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::SymbolPtr);
    auto symbolIndex = stack_.back().val.symbolIndex;
    stack_.pop_back();
    return symbolTable_.at(symbolIndex).name;
}

// ( x -- x )
bool Interpreter::IsAtom()
{
    assert(!stack_.empty());
    return !(stack_.back().type == ObjType::ConsPtr);
}

// ( x -- x )
bool Interpreter::IsNull()
{
    assert(!stack_.empty());
    return (stack_.back().type == ObjType::Null);
}

// ( -- )
StackIndexType Interpreter::StackSize() const
{
    return stack_.size();
}

// ( x -- x )
ObjType Interpreter::Type()
{
    assert(!stack_.empty());
    return stack_.back().type;
}

// ( y x -- x+y )
// Top 2 elements must be Reals
void Interpreter::Add()
{
    assert(stack_.size() >= 2);
    assert(stack_[stack_.size() - 1].type == ObjType::Real);
    assert(stack_[stack_.size() - 2].type == ObjType::Real);
    double x = stack_.back().val.realVal;
    stack_.pop_back();
    stack_.back().val.realVal = stack_.back().val.realVal + x;
}

// ( value var -- )
void Interpreter::AddBinding()
{
    // TODO: Consult the free list rather than always appending to the end

    assert(stack_.size() >= 2);
    assert(!envStack_.empty());

    // Cons the value and var on the stack to make our binding
    Cons();

    // And add it to the current environment
    Object car = stack_.back();
    stack_.pop_back();
    Object& cdr = envStack_.back();
    listMem_.emplace_back(car, cdr);
    // Re-use the top of envStack (cdr)
    cdr.type = ObjType::ConsPtr;
    cdr.val.consIndex = listMem_.size() - 1;
}

// ( arg0 .. argn proc -- val )
// TOS must be a procedure
void Interpreter::Apply(int numArgs)
{
    assert(stack_.size() >= numArgs + 1);
    assert(Type() == ObjType::Expr
           || Type() == ObjType::Fsubr
           || Type() == ObjType::Subr);

    auto prevArgsFrameStart = argsFrameStart_;
    argsFrameStart_ = stack_.size() - numArgs - 1;

    switch (Type()) {
    case ObjType::Expr:
        ApplyExpr(numArgs);
        break;
    case ObjType::Fsubr:
    case ObjType::Subr:
        ApplyCProcedure(numArgs);
        break;
    }

    assert(stack_.size() == argsFrameStart_ + numArgs + 1);

    // Drop the args
    Object val = stack_.back();
    while (stack_.size() > argsFrameStart_) {
        Drop();
    }
    stack_.push_back(val);

    assert(stack_.size() == argsFrameStart_ + 1);

    // Restore argsFrameStart
    argsFrameStart_ = prevArgsFrameStart;
}

// ( key alist -- matched cons or nil )
void Interpreter::Assoc()
{
    // TODO: alist could be non-nil terminated
    assert(stack_.size() >= 2);
    assert(Type() == ObjType::ConsPtr);
    while (!IsNull()) {
        Next();
        if (EqCar(2, 0)) {
            // Leave the matching cons on the stack and return
            Nip();
            Nip();
            return;
        }
        else {
            // This cons didn't match
            Drop();
        }
    }

    // We reached the end of the alist
    // Remove the key and leave nil on the stack
    Nip();
}

// ( cons -- car )
// TOS must be a ConsPtr
void Interpreter::Car()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr);
    // Replace the top of the stack with the cell car
    auto consIndex = stack_.back().val.consIndex;
    stack_.back() = listMem_.at(consIndex).car;
    // TODO: Update reference counts
}

// ( cons -- cdr )
// TOS must be a ConsPtr
void Interpreter::Cdr()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr);
    // Replace the top of the stack with the cell cdr
    auto consIndex = stack_.back().val.consIndex;
    stack_.back() = listMem_.at(consIndex).cdr;
}

// ( cdr car -- cons )
void Interpreter::Cons()
{
    // TODO: Consult the free list rather than always appending to the end
    assert(stack_.size() >= 2);
    Object car = stack_.back();
    stack_.pop_back();
    Object& cdr = stack_.back();
    // TODO: Increase reference count
    listMem_.emplace_back(car, cdr);
    // Re-use the object on the top of the stack (cdr)
    // as a cons pointer to the new cons cell
    cdr.type = ObjType::ConsPtr;
    cdr.val.consIndex = listMem_.size() - 1;
}

void Interpreter::DeleteEnv()
{
    assert(!envStack_.empty());
    envStack_.pop_back();
}

// ( x -- )
void Interpreter::Drop()
{
    assert(!stack_.empty());
    stack_.pop_back();
}

// ( x -- x x )
void Interpreter::Dup()
{
    assert(!stack_.empty());
    stack_.push_back(stack_.back());
    // TODO: Increase reference count if cons
}

// ( y x -- y x )
bool Interpreter::Eq()
{
    assert(stack_.size() >= 2);
    Object x = stack_.back();
    stack_.pop_back();
    Object y = stack_.back();
    stack_.pop_back();
    return Eq(x, y);
}

bool Interpreter::EqCar(StackIndexType index, StackIndexType consIndex)
{
    assert(index >= 0);
    assert(index < stack_.size());
    assert(consIndex >= 0);
    assert(consIndex < stack_.size());
    assert(stack_[stack_.size() - consIndex - 1].type == ObjType::ConsPtr);
    return Eq(stack_[stack_.size() - index - 1],
              listMem_.at(stack_[stack_.size() - consIndex - 1].val.consIndex).car);
}

// ( expr -- value )
void Interpreter::Eval()
{
    assert(!stack_.empty());
    switch (Type()) {
    case ObjType::ConsPtr:
        EvalProcedureCall();
        break;
    case ObjType::SymbolPtr:
        Load();
        break;
    }
}

// ( expr -- body )
// TOS must be an Expr
void Interpreter::ExprBody()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::Expr);
    // Replace the top of the stack with the Expr body
    auto exprIndex = stack_.back().val.exprIndex;
    stack_.back() = listMem_.at(exprIndex).cdr;
}

// ( expr -- params )
// TOS must be an Expr
void Interpreter::ExprParams()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::Expr);
    // Replace the top of the stack with the Expr params
    auto exprIndex = stack_.back().val.exprIndex;
    stack_.back() = listMem_.at(exprIndex).car;
    // TODO: Update reference counts
}

// ( var -- value )
// TOS must be a SymbolPtr
void Interpreter::Load()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::SymbolPtr);
    auto varIndex = stack_.back().val.symbolIndex;
    if (!envStack_.empty()) {
        Object* currPtr = &(envStack_.back());
        while (currPtr->type != ObjType::Null) {
            assert(currPtr->type == ObjType::ConsPtr);
            ConsCell& currCell = listMem_[currPtr->val.consIndex];
            assert(currCell.car.type == ObjType::ConsPtr);
            ConsCell& bindingCell = listMem_[currCell.car.val.consIndex];
            assert(bindingCell.car.type == ObjType::SymbolPtr);
            if (bindingCell.car.val.symbolIndex == varIndex) {
                // We've found a match
                stack_.back() = bindingCell.cdr;
                return;
            }
            // Iterate along the environment list
            currPtr = &(currCell.cdr);
        }
    }
    // We didn't find var in the current environment, look up global
    stack_.back() = symbolTable_.at(varIndex).value;
}

// TODO: Support lambda expressions with more than one body form
// ( body params -- expr )
void Interpreter::MakeExpr()
{
    // Expr structure: (params . body)
    Cons();
    // Repurpose TOS as Expr
    Object& x = stack_.back();
    x.type = ObjType::Expr;
    x.val.exprIndex = x.val.consIndex;
}

// ( y x -- x*y )
// Top 2 elements must be Reals
void Interpreter::Mul()
{
    assert(stack_.size() >= 2);
    assert(stack_[stack_.size() - 1].type == ObjType::Real);
    assert(stack_[stack_.size() - 2].type == ObjType::Real);
    double x = stack_.back().val.realVal;
    stack_.pop_back();
    stack_.back().val.realVal = stack_.back().val.realVal * x;
}

// ( cons -- cdr car )
// TOS must be a ConsPtr
void Interpreter::Next()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr);
    Object& x = stack_.back();
    ConsCell& cell = listMem_.at(x.val.consIndex);
    x = cell.cdr;
    stack_.push_back(cell.car);
}

void Interpreter::NewEnv()
{
    envStack_.emplace_back(ObjType::Null);
}

// ( y x -- x )
void Interpreter::Nip()
{
    assert(stack_.size() >= 2);
    stack_[stack_.size() - 2] = stack_.back();
    stack_.pop_back();
}

// ( x0 .. xn -- x0 .. xn x0 )
void Interpreter::Pick(StackIndexType n)
{
    assert(n >= 0);
    assert(n < stack_.size());
    stack_.push_back(stack_[stack_.size() - n - 1]);
}

// ( -- x )
void Interpreter::PushArg(StackIndexType n)
{
    assert(argsFrameStart_ >= 0);
    assert(n >= 0);
    assert((argsFrameStart_ + n) < stack_.size());
    stack_.push_back(stack_[argsFrameStart_ + n]);
}

// ( value var -- value )
// TOS must be a SymbolPtr
void Interpreter::Store()
{
    assert(stack_.size() >= 2);
    assert(Type() == ObjType::SymbolPtr);
    auto symbolIndex = stack_.back().val.symbolIndex;
    stack_.pop_back();
    symbolTable_.at(symbolIndex).value = stack_.back();
}

// ( y x -- x y )
void Interpreter::Swap()
{
    assert(stack_.size() >= 2);
    auto stackSize = stack_.size();
    std::swap(stack_[stackSize - 1], stack_[stackSize - 2]);
}

// ( x -- )
std::string Interpreter::PrintToString()
{
    assert(!stack_.empty());
    return printer_.PrintToString(this);
}

// ( -- x )
void Interpreter::Read(const std::string& str)
{
    Reader reader(this, str);
    reader.Read();
}

// ( arg0 .. argn proc -- val )
// argsFrameStart must have been set
void Interpreter::ApplyCProcedure(int numArgs)
{
    // TODO: I think that argsFrameStart is only needed for CProcedures
    //       so move that logic from Apply() to this function. For Lisp
    //       procedures, we will use an environment. Or useful to have
    //       available for Lisp procedures also?

    assert(numArgs >= 0);
    assert(argsFrameStart_ >= 0);
    assert(stack_.size() == argsFrameStart_ + numArgs + 1);
    assert(Type() == ObjType::Fsubr || Type() == ObjType::Subr);

    CProcedure proc = stack_.back().val.proc;
    stack_.pop_back();
    proc(this, numArgs);
}

// ( arg0 .. argn expr -- val)
// argsFrameStart must have been set
void Interpreter::ApplyExpr(int numArgs)
{
    // TODO: Verify called with the expected number of args
    // TODO: Support Exprs with more than one form in their body (Progn)

    assert(numArgs >= 0);
    assert(argsFrameStart_ >= 0);
    assert(stack_.size() == argsFrameStart_ + numArgs + 1);
    assert(Type() == ObjType::Expr);

    // Set up the environment for this call
    NewEnv();
    Dup();
    ExprParams();
    int paramNum = 0;
    while (!IsNull()) {
        Next();
        PushArg(paramNum);
        Swap();
        AddBinding();
        ++paramNum;
    }
    Drop(); // The params list terminating nil

    // Eval body
    ExprBody();
    Eval();

    // Pop the environment
    DeleteEnv();
}

bool Interpreter::Eq(const Object& x, const Object& y)
{
    if (x.type != y.type) {
        return false;
    }
    switch (x.type) {
    case ObjType::SymbolPtr:
        return (x.val.symbolIndex == y.val.symbolIndex);
        // TODO: Other object types
    case ObjType::ConsPtr:
        return (x.val.consIndex == x.val.consIndex);
    }
    return false;
}

// ( call -- value )
// TOS must be a procedure call form
void Interpreter::EvalProcedureCall()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr);

    // Eval operator
    Next();
    Eval();

    bool evalArgs = false;

    switch (Type()) {
    case ObjType::Expr:
    case ObjType::Subr:
        evalArgs = true;
    case ObjType::Fsubr: {
        Swap();
        // Expand the args list
        int numArgs = ListElems(evalArgs);
        // Push the operator onto the top of the stack
        Pick(numArgs);
        // Apply operator
        Apply(numArgs);
        break;
    }
    default:
        // TODO: Complain -- unsupported operator type
        break;
    }

    // Remove the operator left on the stack
    Nip();
}

// ( list -- elem0 .. elemn )
int Interpreter::ListElems(bool evalElems)
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr || Type() == ObjType::Null);

    int numElems = 0;
    while (!IsNull()) {
        Next();
        if (evalElems) {
            Eval();
        }
        Swap();
        ++numElems;
    }
    Drop(); // The terminating nil
    return numElems;
}

void Interpreter::StoreFsubr(const std::string& var, CProcedure proc)
{
    PushFsubr(proc);
    PushSymbol(var);
    Store();
    Drop();
}

void Interpreter::StoreSubr(const std::string& var, CProcedure proc)
{
    PushSubr(proc);
    PushSymbol(var);
    Store();
    Drop();
}

} // namespace Procdraw
