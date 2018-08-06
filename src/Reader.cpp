/* Copyright (c) 2018 Simon Bates
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx.h"
#include "Reader.h"
#include "Interpreter.h"
#include <cctype>
#include <stdexcept>
#include <string>

namespace Procdraw {

Reader::Reader(Interpreter* interpreter, const std::string& str)
    : interpreter_(interpreter)
{
    input_.str(str);
    GetCh();
    GetToken();
}

void Reader::Read()
{
    switch (token_) {
    case ReaderTokenType::LParen:
        ReadCons();
        break;
    case ReaderTokenType::Real:
        interpreter_->PushReal(realVal_);
        GetToken();
        break;
    case ReaderTokenType::Symbol:
        if (symbolName_ == "nil") {
            interpreter_->PushNil();
        }
        else if (symbolName_ == "true") {
            interpreter_->PushBoolean(true);
        }
        else if (symbolName_ == "false") {
            interpreter_->PushBoolean(false);
        }
        else {
            interpreter_->PushSymbol(symbolName_);
        }
        GetToken();
        break;
    default:
        // TODO: Custom syntax error type
        throw std::runtime_error("Syntax Error");
    }
}

void Reader::GetCh()
{
    ch_ = input_.get();
}

void Reader::GetToken()
{
    while (isspace(ch_)) {
        GetCh();
    }

    switch (ch_) {
    case '(':
        token_ = ReaderTokenType::LParen;
        GetCh();
        break;
    case ')':
        token_ = ReaderTokenType::RParen;
        GetCh();
        break;
    case '*':
    case '+':
        token_ = ReaderTokenType::Symbol;
        symbolName_ = std::string(1, ch_);
        GetCh();
        break;
    case '.':
        token_ = ReaderTokenType::Dot;
        GetCh();
        break;
    case EOF:
        token_ = ReaderTokenType::EndOfInput;
        break;
    default:
        if (isdigit(ch_)) {
            GetReal();
        }
        else if (isalpha(ch_)) {
            GetSymbol();
        }
        else {
            token_ = ReaderTokenType::Undefined;
            GetCh();
        }
        break;
    }
}

void Reader::GetReal()
{
    std::string real;
    while (isdigit(ch_)) {
        real += ch_;
        GetCh();
    }
    token_ = ReaderTokenType::Real;
    realVal_ = atoi(real.c_str());
}

void Reader::GetSymbol()
{
    std::string name;
    while (isalnum(ch_) || ch_ == '-') {
        name += ch_;
        GetCh();
    }
    token_ = ReaderTokenType::Symbol;
    symbolName_ = name;
}

void Reader::Expect(ReaderTokenType t)
{
    if (token_ == t) {
        GetToken();
    }
    else {
        // TODO custom syntax error type
        throw std::runtime_error("Syntax error");
    }
}

void Reader::ReadCons()
{
    GetToken(); // Step over LParen

    bool done = (token_ == ReaderTokenType::EndOfInput);
    bool expressionClosed = false;
    int elementCount = 0;

    while (!done) {
        switch (token_) {
        case ReaderTokenType::EndOfInput:
            done = true;
            break;
        case ReaderTokenType::RParen:
            GetToken();
            // We have a proper list, push nil
            interpreter_->PushNil();
            expressionClosed = true;
            done = true;
            break;
        case ReaderTokenType::Dot:
            GetToken();
            // We have a dotted pair, push the terminating cdr
            Read();
            Expect(ReaderTokenType::RParen);
            expressionClosed = true;
            done = true;
            break;
        default:
            // Read the next element and push it onto the stack
            Read();
            ++elementCount;
            break;
        }
    }

    if (expressionClosed) {
        // Cons the elements on the stack
        for (int i = 0; i < elementCount; ++i) {
            interpreter_->Swap();
            interpreter_->Cons();
        }
    }
    else {
        // TODO: Need to tidy up the stack
        // TODO: Custom syntax error type
        throw std::runtime_error("Syntax Error");
    }
}
} // namespace Procdraw
