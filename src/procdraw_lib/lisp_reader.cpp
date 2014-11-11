#include "lisp_reader.h"
#include "lisp_interpreter.h"
#include <cctype>

namespace procdraw {

    LispObjectPtr LispReader::Read(LispInterpreter *L, const std::string &str)
    {
        input_.str(str);
        input_.clear();
        GetCh();
        GetToken();
        return Read(L);
    }

    void LispReader::GetCh()
    {
        ch_ = input_.get();
    }

    void LispReader::GetToken()
    {
        while (isspace(ch_)) {
            GetCh();
        }

        if (ch_ == EOF) {
            token_ = LispTokenType::EndOfInput;
        }
        else if (ch_ == '(') {
            token_ = LispTokenType::LParen;
            GetCh();
        }
        else if (ch_ == ')') {
            token_ = LispTokenType::RParen;
            GetCh();
        }
        else if (ch_ == '.') {
            token_ = LispTokenType::Dot;
            GetCh();
        }
        else if (isdigit(ch_)) {
            std::string number;
            while (isdigit(ch_)) {
                number += ch_;
                GetCh();
            }
            token_ = LispTokenType::Number;
            numVal_ = atoi(number.c_str());
        }
        else if (isalpha(ch_)) {
            std::string str;
            while (isalnum(ch_) || ch_ == '-') {
                str += ch_;
                GetCh();
            }
            token_ = LispTokenType::Symbol;
            symbolVal_ = str;
        }
        else {
            token_ = LispTokenType::Undefined;
        }
    }

    LispObjectPtr LispReader::Read(LispInterpreter *L)
    {
        if (token_ == LispTokenType::LParen) {
            GetToken();
            return ReadCons(L);
        }
        else if (token_ == LispTokenType::Number) {
            auto intObj = L->MakeNumber(numVal_);
            GetToken();
            return intObj;
        }
        else if (token_ == LispTokenType::Symbol) {
            LispObjectPtr obj;
            if (symbolVal_ == "nil") {
                obj = L->Nil;
            }
            else {
                obj = L->SymbolRef(symbolVal_);
            }
            GetToken();
            return obj;
        }
        // TODO LispTokenType::EndOfInput
        // TODO badly formed input
        // TODO error reporting strategy
        throw std::runtime_error("Bad input at LispReader::Read()");
    }

    LispObjectPtr LispReader::ReadCons(LispInterpreter *L)
    {
        if (token_ == LispTokenType::RParen) {
            // Empty list
            GetToken();
            return L->Nil;
        }
        auto car = Read(L);
        if (token_ == LispTokenType::RParen) {
            GetToken();
            return L->Cons(car, L->Nil);
        }
        else if (token_ == LispTokenType::Dot) {
            GetToken();
            return L->Cons(car, Read(L));
        }
        else {
            return L->Cons(car, ReadCons(L));
        }
    }

}
