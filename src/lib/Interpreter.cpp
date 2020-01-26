// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Interpreter.h"
#include <algorithm>
#include <string>

#define FOLD_LEFT_INT(accumulator, operation, lst, initial)             \
    int accumulator = initial;                                          \
    ListPtr next = lst;                                                 \
    while (next != nullptr) {                                           \
        accumulator = accumulator operation next->First().GetInteger(); \
        next = next->Rest();                                            \
    }

namespace Procdraw {

Object SubrProduct(Interpreter* interpreter, ListPtr args)
{
    FOLD_LEFT_INT(product, *, args, 1)
    return Object{product};
}

Object SubrSum(Interpreter* interpreter, ListPtr args)
{
    FOLD_LEFT_INT(sum, +, args, 0)
    return Object{sum};
}

Interpreter::Interpreter()
{
    printer = std::make_unique<Printer>(this);
    reader = std::make_unique<Reader>(this);

    functions.resize(2);
    functions.at(0) = SubrProduct;
    SetSymbolValue(SymbolRef("*"), Object::MakeCFunctionHandle(0));
    functions.at(1) = SubrSum;
    SetSymbolValue(SymbolRef("+"), Object::MakeCFunctionHandle(1));
}

Object Interpreter::Apply(const Object& fun, ListPtr args)
{
    return functions.at(fun.GetCFunctionHandle())(this, args);
}

Object Interpreter::Eval(const Object& expr)
{
    switch (expr.Type()) {
    case ObjectType::Boolean:
    case ObjectType::Integer:
    case ObjectType::None:
        return expr;
    case ObjectType::SymbolHandle:
        return SymbolValue(expr.GetSymbolHandle());
    case ObjectType::ListPtr: {
        Object first = expr.GetListPtr()->First();
        return Apply(Eval(first), expr.GetListPtr()->Rest());
    }
    default:
        throw std::exception{"Unhandled type in Eval"};
    }
}

std::string Interpreter::Print(const Object& obj) const
{
    return this->printer->Print(obj);
}

Object Interpreter::Read(const std::string& text)
{
    return this->reader->Read(text);
}

void Interpreter::SetSymbolValue(SymbolHandle handle, const Object& value)
{
    symbols.at(handle).value = value;
}

std::string Interpreter::SymbolName(SymbolHandle handle) const
{
    return symbols.at(handle).name;
}

Object Interpreter::SymbolValue(SymbolHandle handle) const
{
    return symbols.at(handle).value;
}

SymbolHandle Interpreter::SymbolRef(const std::string& name)
{
    auto it = std::find_if(
        symbols.begin(),
        symbols.end(),
        [name](const Symbol& s) -> bool { return s.name == name; });

    if (it == symbols.end()) {
        symbols.emplace_back(name);
        return symbols.size() - 1;
    }
    else {
        return std::distance(symbols.begin(), it);
    }
}

} // namespace Procdraw
