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

#ifndef PROCDRAW_READER_H
#define PROCDRAW_READER_H

#include "InterpreterTypes.h"
#include <sstream>
#include <string>

namespace Procdraw {

class Interpreter;

enum class ReaderTokenType {
    LParen,
    RParen,
    Integer,
    Symbol,
    EndOfInput,
    Undefined
};

class SyntaxError : public std::exception {
public:
    const char* what() const override { return "Syntax Error"; }
};

class Reader {
public:
    explicit Reader(Interpreter* interpreter);
    Object Read(const std::string& text);

private:
    Interpreter* interpreter;
    std::istringstream input;
    int ch;
    ReaderTokenType token;
    int intVal;
    std::string symbolVal;
    void SetInput(const std::string& text);
    void GetCh();
    bool IsStartOfNumber();
    void GetNumber();
    void GetToken();
    Object Read();
    ListPtr ReadCons();
};

} // namespace Procdraw

#endif
