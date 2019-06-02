/* Copyright (c) 2018, 2019 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pch.h"
#include "VirtualMachine.h"
#include <cassert>

namespace Procdraw {

// ( -- boolean )
void VirtualMachine::PushBoolean(bool val)
{
    Object obj(ObjType::Boolean);
    obj.val.boolVal = val;
    stack_.push_back(obj);
}

// ( x -- )
bool VirtualMachine::PopBoolean()
{
    assert(!stack_.empty());
    // TODO: Convert to boolean
    bool val = stack_.back().val.boolVal;
    stack_.pop_back();
    return val;
}

// ( -- fsubr )
void VirtualMachine::PushFsubr(SysFunc sysFunc)
{
    Object obj(ObjType::Fsubr);
    obj.val.sysFunc = sysFunc;
    stack_.push_back(obj);
}

// ( -- nil )
void VirtualMachine::PushNil()
{
    stack_.emplace_back(ObjType::Null);
}

// ( -- real )
void VirtualMachine::PushReal(double val)
{
    Object obj(ObjType::Real);
    obj.val.realVal = val;
    stack_.push_back(obj);
}

// ( real -- )
// TOS must be a Real
double VirtualMachine::PopReal()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::Real);
    double val = stack_.back().val.realVal;
    stack_.pop_back();
    return val;
}

// ( -- subr )
void VirtualMachine::PushSubr(SysFunc sysFunc)
{
    Object obj(ObjType::Subr);
    obj.val.sysFunc = sysFunc;
    stack_.push_back(obj);
}

// ( -- symbol )
void VirtualMachine::PushSymbol(const std::string& name)
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
std::string VirtualMachine::PopSymbol()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::SymbolPtr);
    auto symbolIndex = stack_.back().val.symbolIndex;
    stack_.pop_back();
    return symbolTable_.at(symbolIndex).name;
}

// ( x -- x )
bool VirtualMachine::IsAtom()
{
    assert(!stack_.empty());
    return !(stack_.back().type == ObjType::ConsPtr);
}

// ( x -- x )
bool VirtualMachine::IsNull()
{
    assert(!stack_.empty());
    return (stack_.back().type == ObjType::Null);
}

// ( -- )
StackIndexType VirtualMachine::StackSize() const
{
    return stack_.size();
}

// ( x -- x )
ObjType VirtualMachine::Type()
{
    assert(!stack_.empty());
    return stack_.back().type;
}

// ( y x -- x+y )
// Top 2 elements must be Reals
void VirtualMachine::Add()
{
    assert(stack_.size() >= 2);
    assert(stack_[stack_.size() - 1].type == ObjType::Real);
    assert(stack_[stack_.size() - 2].type == ObjType::Real);
    double x = stack_.back().val.realVal;
    stack_.pop_back();
    stack_.back().val.realVal = stack_.back().val.realVal + x;
}

// ( value var -- )
void VirtualMachine::AddBinding()
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

// ( arg0 .. argn func -- val )
void VirtualMachine::CallSysFunc(int numArgs, Interpreter* interpreter)
{
    assert(numArgs >= 0);
    assert(StackSize() >= static_cast<unsigned>(numArgs) + 1);
    assert(Type() == ObjType::Fsubr || Type() == ObjType::Subr);

    // Save current framePointer_
    auto prevFramePointer = framePointer_;
    // Set framePointer_
    framePointer_ = stack_.size() - numArgs - 1;

    // Call the function
    SysFunc func = stack_.back().val.sysFunc;
    stack_.pop_back();
    func(numArgs, interpreter);

    // Save the result on the aux stack
    ToAux();

    // Clean up the stack
    while (stack_.size() > framePointer_) {
        Drop();
    }

    // Put back the result
    FromAux();

    // Restore framePointer_
    framePointer_ = prevFramePointer;
}

// ( cons -- car )
// TOS must be a ConsPtr
void VirtualMachine::Car()
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
void VirtualMachine::Cdr()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr);
    // Replace the top of the stack with the cell cdr
    auto consIndex = stack_.back().val.consIndex;
    stack_.back() = listMem_.at(consIndex).cdr;
}

// ( cdr car -- cons )
void VirtualMachine::Cons()
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

// ( x -- )
void VirtualMachine::Drop()
{
    assert(!stack_.empty());
    stack_.pop_back();
}

void VirtualMachine::DropEnv()
{
    assert(!envStack_.empty());
    envStack_.pop_back();
}

// ( x -- x x )
void VirtualMachine::Dup()
{
    assert(!stack_.empty());
    stack_.push_back(stack_.back());
    // TODO: Increase reference count if cons
}

// ( y x -- y x )
bool VirtualMachine::Eq()
{
    assert(stack_.size() >= 2);
    Object x = stack_.back();
    stack_.pop_back();
    Object y = stack_.back();
    stack_.pop_back();
    return Eq(x, y);
}

bool VirtualMachine::EqCar(StackIndexType index, StackIndexType consIndex)
{
    assert(index >= 0);
    assert(index < stack_.size());
    assert(consIndex >= 0);
    assert(consIndex < stack_.size());
    assert(stack_[stack_.size() - consIndex - 1].type == ObjType::ConsPtr);
    return Eq(stack_[stack_.size() - index - 1],
              listMem_.at(stack_[stack_.size() - consIndex - 1].val.consIndex).car);
}

// ( expr -- body )
// TOS must be an Expr
void VirtualMachine::ExprBody()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::Expr);
    // Replace the top of the stack with the Expr body
    auto exprIndex = stack_.back().val.exprIndex;
    stack_.back() = listMem_.at(exprIndex).cdr;
}

// ( expr -- params )
// TOS must be an Expr
void VirtualMachine::ExprParams()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::Expr);
    // Replace the top of the stack with the Expr params
    auto exprIndex = stack_.back().val.exprIndex;
    stack_.back() = listMem_.at(exprIndex).car;
    // TODO: Update reference counts
}

// ( -- obj )
void VirtualMachine::FromAux()
{
    assert(!auxStack_.empty());
    stack_.push_back(auxStack_.back());
    auxStack_.pop_back();
}

// ( var -- value )
// TOS must be a SymbolPtr
void VirtualMachine::Load()
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
void VirtualMachine::MakeExpr()
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
void VirtualMachine::Mul()
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
void VirtualMachine::Next()
{
    assert(!stack_.empty());
    assert(Type() == ObjType::ConsPtr);
    Object& x = stack_.back();
    ConsCell& cell = listMem_.at(x.val.consIndex);
    x = cell.cdr;
    stack_.push_back(cell.car);
}

void VirtualMachine::NewEnv()
{
    envStack_.emplace_back(ObjType::Null);
}

// ( y x -- x )
void VirtualMachine::Nip()
{
    assert(stack_.size() >= 2);
    stack_[stack_.size() - 2] = stack_.back();
    stack_.pop_back();
}

// ( x0 .. xn -- x0 .. xn x0 )
void VirtualMachine::Pick(StackIndexType n)
{
    assert(n >= 0);
    assert(n < stack_.size());
    stack_.push_back(stack_[stack_.size() - n - 1]);
}

// ( -- x )
void VirtualMachine::PushArg(StackIndexType n)
{
    assert(framePointer_ >= 0);
    assert(n >= 0);
    assert((framePointer_ + n) < stack_.size());
    stack_.push_back(stack_[framePointer_ + n]);
}

// ( value var -- value )
// TOS must be a SymbolPtr
void VirtualMachine::Store()
{
    assert(stack_.size() >= 2);
    assert(Type() == ObjType::SymbolPtr);
    auto symbolIndex = stack_.back().val.symbolIndex;
    stack_.pop_back();
    symbolTable_.at(symbolIndex).value = stack_.back();
}

// ( y x -- y-x )
// Top 2 elements must be Reals
void VirtualMachine::Sub()
{
    assert(stack_.size() >= 2);
    assert(stack_[stack_.size() - 1].type == ObjType::Real);
    assert(stack_[stack_.size() - 2].type == ObjType::Real);
    double x = stack_.back().val.realVal;
    stack_.pop_back();
    stack_.back().val.realVal = stack_.back().val.realVal - x;
}

// ( y x -- x y )
void VirtualMachine::Swap()
{
    assert(stack_.size() >= 2);
    auto stackSize = stack_.size();
    std::swap(stack_[stackSize - 1], stack_[stackSize - 2]);
}

// ( obj -- )
void VirtualMachine::ToAux()
{
    assert(!stack_.empty());
    auxStack_.push_back(stack_.back());
    stack_.pop_back();
}

bool VirtualMachine::Eq(const Object& x, const Object& y)
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

} // namespace Procdraw
