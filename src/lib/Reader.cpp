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

#include "Reader.h"
#include "Interpreter.h"
#include <cctype>
#include <string>

namespace Procdraw {

Reader::Reader(Procdraw::Interpreter* interpreter)
{
    this->interpreter = interpreter;
}

Object Reader::Read(const std::string& text)
{
    SetInput(text);
    return Read();
}

void Reader::SetInput(const std::string& text)
{
    input.str(text);
    input.clear();
    GetCh();
    GetToken();
}

void Reader::GetCh()
{
    ch = input.get();
}

bool Reader::IsStartOfNumber()
{
    return isdigit(ch) != 0;
}

void Reader::GetNumber()
{
    std::string number;
    while (isdigit(ch)) {
        number += ch;
        GetCh();
    }
    token = ReaderTokenType::Integer;
    intVal = atoi(number.c_str());
}

void Reader::GetToken()
{
    while (isspace(ch)) {
        GetCh();
    }

    switch (ch) {
    case '(':
        token = ReaderTokenType::LParen;
        GetCh();
        break;
    case ')':
        token = ReaderTokenType::RParen;
        GetCh();
        break;
    case '+':
        GetCh();
        if (IsStartOfNumber()) {
            GetNumber();
        }
        else {
            token = ReaderTokenType::Symbol;
            symbolVal = "+";
        }
        break;
    // Single char symbols
    case '*':
        token = ReaderTokenType::Symbol;
        symbolVal = std::string(1, ch);
        GetCh();
        break;
    case EOF:
        token = ReaderTokenType::EndOfInput;
        break;
    default:
        if (IsStartOfNumber()) {
            GetNumber();
        }
        else if (isalpha(ch)) {
            std::string str;
            while (isalnum(ch) || ch == '-') {
                str += ch;
                GetCh();
            }
            token = ReaderTokenType::Symbol;
            symbolVal = str;
        }
        else {
            token = ReaderTokenType::Undefined;
            GetCh();
        }
        break;
    }
}

Object Reader::Read()
{
    switch (token) {
    case ReaderTokenType::LParen:
        return ReadCons();
    case ReaderTokenType::Integer: {
        Object obj{intVal};
        GetToken();
        return obj;
    }
    case ReaderTokenType::Symbol:
        if (symbolVal == "true") {
            GetToken();
            return true;
        }
        else if (symbolVal == "false") {
            GetToken();
            return false;
        }
        else if (symbolVal == "none") {
            GetToken();
            return Object::None();
        }
        else {
            Object obj = Object::MakeSymbolHandle(interpreter->SymbolRef(symbolVal));
            GetToken();
            return obj;
        }
    default:
        throw SyntaxError{};
    }
}

ListPtr Reader::ReadCons()
{
    // Consume LParen
    GetToken();

    if (token == ReaderTokenType::RParen) {
        GetToken();
        // Empty list
        return nullptr;
    }

    ListPtr head = Cons(Read(), nullptr);
    ListPtr prev = head;

    while (token != ReaderTokenType::EndOfInput) {
        if (token == ReaderTokenType::RParen) {
            GetToken();
            return head;
        }
        ListPtr next = Cons(Read(), nullptr);
        prev->SetRest(next);
        prev = next;
    }

    // Unterminated list
    throw SyntaxError{};
}

} // namespace Procdraw
