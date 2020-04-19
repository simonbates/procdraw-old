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

#pragma once

#include "InterpreterTypes.h"
#include "Printer.h"
#include "Reader.h"
#include <memory>
#include <string>
#include <vector>

// Note: It is not safe to share Objects between Interpreter instances as
//       Objects may have handles into Interpreter-specific data structures,
//       such as a symbol table.

namespace Procdraw {

struct Symbol {
    explicit Symbol(const std::string& name)
        : name(name), value(Object::None()) {}
    std::string name;
    Object value;
};

class Interpreter;

typedef Object (*CFunction)(Interpreter* interpreter, ListPtr args);

class Interpreter {
public:
    Interpreter();
    Object Apply(const Object& fun, ListPtr args);
    Object Eval(const Object& expr);
    std::string Print(const Object& obj) const;
    Object Read(const std::string& text);
    void SetSymbolValue(SymbolHandle handle, const Object& value);
    std::string SymbolName(SymbolHandle handle) const;
    SymbolHandle SymbolRef(const std::string& name);
    Object SymbolValue(SymbolHandle handle) const;

private:
    std::unique_ptr<Printer> printer;
    std::unique_ptr<Reader> reader;
    std::vector<Symbol> symbols;
    std::vector<CFunction> functions;
};

} // namespace Procdraw
