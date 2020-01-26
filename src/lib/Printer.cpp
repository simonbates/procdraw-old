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

#include "Printer.h"
#include "Interpreter.h"
#include <iterator>
#include <sstream>

namespace Procdraw {

Printer::Printer(Interpreter* interpreter)
{
    this->interpreter = interpreter;
}

std::string Printer::Print(const Object& obj)
{
    switch (obj.Type()) {
    case ObjectType::Boolean:
        return obj.GetBoolean() ? "true" : "false";
    case ObjectType::Integer: {
        std::ostringstream s;
        s << obj.GetInteger();
        return s.str();
    }
    case ObjectType::ListPtr: {
        std::string s{"("};
        ListPtr next{obj.GetListPtr()};
        bool firstChild{true};
        while (next != nullptr) {
            if (firstChild) {
                firstChild = false;
            }
            else {
                s.push_back(' ');
            }
            s.append(Print(next->First()));
            next = next->Rest();
        }
        s.append(")");
        return s;
    }
    case ObjectType::None:
        return "none";
    case ObjectType::SymbolHandle:
        return interpreter->SymbolName(obj.GetSymbolHandle());
    default:
        throw std::exception{"Unhandled type in Print"};
    }
}

} // namespace Procdraw
