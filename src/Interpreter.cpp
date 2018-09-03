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

void SubrDifference(Interpreter* interpreter, int numArgs)
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
    StoreSubr("-", SubrDifference);
}

// ( arg0 .. argn proc -- val )
// TOS must be a procedure
void Interpreter::Apply(int numArgs)
{
    assert(StackSize() >= numArgs + 1);
    assert(Type() == ObjType::Expr
           || Type() == ObjType::Fsubr
           || Type() == ObjType::Subr);

    auto prevArgsFrameStart = argsFrameStart_;
    argsFrameStart_ = StackSize() - numArgs - 1;

    switch (Type()) {
    case ObjType::Expr:
        ApplyExpr(numArgs);
        break;
    case ObjType::Fsubr:
    case ObjType::Subr:
        ApplyCProcedure(this, numArgs);
        break;
    }

    assert(StackSize() == argsFrameStart_ + numArgs + 1);

    // Save the result on the aux stack
    ToAux();
    // Drop the args
    while (StackSize() > argsFrameStart_) {
        Drop();
    }
    // Put back the result
    FromAux();

    assert(StackSize() == argsFrameStart_ + 1);

    // Restore argsFrameStart_
    argsFrameStart_ = prevArgsFrameStart;
}

// ( key alist -- matched cons or nil )
void Interpreter::Assoc()
{
    // TODO: alist could be non-nil terminated
    assert(StackSize() >= 2);
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

// ( expr -- value )
void Interpreter::Eval()
{
    assert(StackSize() > 0);
    switch (Type()) {
    case ObjType::ConsPtr:
        EvalProcedureCall();
        break;
    case ObjType::SymbolPtr:
        Load();
        break;
    }
}

// ( x -- )
std::string Interpreter::PrintToString()
{
    assert(StackSize() > 0);
    return printer_.PrintToString(this);
}

// ( -- x )
void Interpreter::Read(const std::string& str)
{
    Reader reader(this, str);
    reader.Read();
}

// ( arg0 .. argn expr -- arg0 .. argn val)
// argsFrameStart must have been set
void Interpreter::ApplyExpr(int numArgs)
{
    // TODO: Verify called with the expected number of args
    // TODO: Support Exprs with more than one form in their body (Progn)

    assert(numArgs >= 0);
    assert(argsFrameStart_ >= 0);
    assert(StackSize() == argsFrameStart_ + numArgs + 1);
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

// ( call -- value )
// TOS must be a procedure call form
void Interpreter::EvalProcedureCall()
{
    assert(StackSize() > 0);
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
    assert(StackSize() > 0);
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
