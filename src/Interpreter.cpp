/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "Interpreter.h"
#include <cassert>
#include <utility>

namespace Procdraw {

void FsubrLambda(int numArgs, Interpreter* interpreter)
{
    // TODO: Support lambda expressions with more than one body form
    interpreter->PushArg(1); // body
    interpreter->PushArg(0); // args
    interpreter->MakeExpr();
}

void FsubrSet(int numArgs, Interpreter* interpreter)
{
    // TODO: Check number and types of arguments
    interpreter->PushArg(1); // expr
    interpreter->Eval();
    interpreter->PushArg(0); // var
    interpreter->Store();
}

void SubrDifference(int numArgs, Interpreter* interpreter)
{
    // TODO difference with 0 args? return a value or complain that not enough
    // args?

    if (numArgs == 0) {
        interpreter->PushReal(0.0);
    }
    else if (numArgs == 1) {
        interpreter->PushReal(0.0);
        interpreter->PushArg(0);
        interpreter->Sub();
    }
    else {
        interpreter->PushArg(0);
        for (int i = 1; i < numArgs; ++i) {
            interpreter->PushArg(i);
            interpreter->Sub();
        }
    }
}

void SubrProduct(int numArgs, Interpreter* interpreter)
{
    // TODO: Check arguments are all Reals
    interpreter->PushReal(1.0);
    for (int i = 0; i < numArgs; ++i) {
        interpreter->PushArg(i);
        interpreter->Mul();
    }
}

void SubrSum(int numArgs, Interpreter* interpreter)
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
    StoreSubr("-", SubrDifference);
}

// ( key alist -- matched cons or nil )
void Interpreter::Assoc()
{
    // TODO: alist could be non-nil terminated
    assert(vm_.StackSize() >= 2);
    assert(vm_.Type() == ObjType::ConsPtr);
    while (!vm_.IsNull()) {
        vm_.Next();
        if (vm_.EqCar(2, 0)) {
            // Leave the matching cons on the stack and return
            vm_.Nip();
            vm_.Nip();
            return;
        }
        else {
            // This cons didn't match
            vm_.Drop();
        }
    }

    // We reached the end of the alist
    // Remove the key and leave nil on the stack
    vm_.Nip();
}

// ( arg0 .. argn proc -- val )
// TOS must be a procedure
void Interpreter::Call(int numArgs)
{
    assert(numArgs >= 0);
    assert(vm_.StackSize() >= static_cast<unsigned>(numArgs) + 1);
    assert(vm_.Type() == ObjType::Expr
           || vm_.Type() == ObjType::Fsubr
           || vm_.Type() == ObjType::Subr);

    switch (vm_.Type()) {
    case ObjType::Expr:
        CallExpr(numArgs);
        break;
    case ObjType::Fsubr:
    case ObjType::Subr:
        vm_.CallSysFunc(numArgs, this);
        break;
    }
}

// ( expr -- value )
void Interpreter::Eval()
{
    assert(vm_.StackSize() > 0);
    switch (vm_.Type()) {
    case ObjType::ConsPtr:
        EvalProcedureCall();
        break;
    case ObjType::SymbolPtr:
        vm_.Load();
        break;
    }
}

// ( x -- )
std::string Interpreter::PrintToString()
{
    assert(vm_.StackSize() > 0);
    return printer_.PrintToString(this);
}

// ( -- x )
void Interpreter::Read(const std::string& str)
{
    Reader reader(this, str);
    reader.Read();
}

// ( arg0 .. argn expr -- val)
void Interpreter::CallExpr(int numArgs)
{
    // TODO: Verify called with the expected number of args
    // TODO: Support Exprs with more than one form in their body (Progn)

    assert(numArgs >= 0);
    assert(vm_.StackSize() >= static_cast<unsigned>(numArgs) + 1);
    assert(vm_.Type() == ObjType::Expr);

    // Save current framePointer_
    auto prevFramePointer = vm_.framePointer_;
    // Set framePointer_
    vm_.framePointer_ = vm_.StackSize() - numArgs - 1;

    // Set up the environment for this call
    vm_.NewEnv();
    vm_.Dup();
    vm_.ExprParams();
    int paramNum = 0;
    while (!vm_.IsNull()) {
        vm_.Next();
        vm_.PushArg(paramNum);
        vm_.Swap();
        vm_.AddBinding();
        ++paramNum;
    }
    vm_.Drop(); // The params list terminating nil

    // Eval body
    vm_.ExprBody();
    Eval();

    // Drop the environment
    vm_.DropEnv();

    // Save the result on the aux stack
    vm_.ToAux();

    // Clean up the stack
    while (vm_.StackSize() > vm_.framePointer_) {
        vm_.Drop();
    }

    // Put back the result
    vm_.FromAux();

    // Restore framePointer_
    vm_.framePointer_ = prevFramePointer;
}

// ( call -- value )
// TOS must be a procedure call form
void Interpreter::EvalProcedureCall()
{
    assert(vm_.StackSize() > 0);
    assert(vm_.Type() == ObjType::ConsPtr);

    // Eval operator
    vm_.Next();
    Eval();

    bool evalArgs = false;

    switch (vm_.Type()) {
    case ObjType::Expr:
    case ObjType::Subr:
        evalArgs = true;
    case ObjType::Fsubr: {
        vm_.Swap();
        // Expand the args list
        int numArgs = ListElems(evalArgs);
        // Push the operator onto the top of the stack
        vm_.Pick(numArgs);
        // Call operator
        Call(numArgs);
        break;
    }
    default:
        // TODO: Complain -- unsupported operator type
        break;
    }

    // Remove the operator left on the stack
    vm_.Nip();
}

// ( list -- elem0 .. elemn )
int Interpreter::ListElems(bool evalElems)
{
    assert(vm_.StackSize() > 0);
    assert(vm_.Type() == ObjType::ConsPtr || vm_.Type() == ObjType::Null);

    int numElems = 0;
    while (!vm_.IsNull()) {
        vm_.Next();
        if (evalElems) {
            Eval();
        }
        vm_.Swap();
        ++numElems;
    }
    vm_.Drop(); // The terminating nil
    return numElems;
}

void Interpreter::StoreFsubr(const std::string& var, SysFunc sysFunc)
{
    vm_.PushFsubr(sysFunc);
    vm_.PushSymbol(var);
    vm_.Store();
    vm_.Drop();
}

void Interpreter::StoreSubr(const std::string& var, SysFunc sysFunc)
{
    vm_.PushSubr(sysFunc);
    vm_.PushSymbol(var);
    vm_.Store();
    vm_.Drop();
}

} // namespace Procdraw
