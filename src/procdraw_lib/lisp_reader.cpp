#include "lisp_reader.h"
#include <cctype>

namespace procdraw {

    LispObjectPtr LispReader::Read(const std::string &str)
    {
        input_ = std::istringstream(str);
        GetCh();
        GetToken();
        return Read();
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

    LispObjectPtr LispReader::Read()
    {
        if (token_ == LispTokenType::LParen) {
            GetToken();
            return ReadCons();
        }
        else if (token_ == LispTokenType::Number) {
            auto intObj = mem_.MakeNumber(numVal_);
            GetToken();
            return intObj;
        }
        else if (token_ == LispTokenType::Symbol) {
            auto symbolObj = mem_.MakeSymbol(symbolVal_);
            GetToken();
            return symbolObj;
        }
        // TODO LispTokenType::EndOfInput
        // TODO badly formed input
        // TODO error reporting strategy
        throw std::runtime_error("Bad input at LispReader::Read()");
    }

    LispObjectPtr LispReader::ReadCons()
    {
        if (token_ == LispTokenType::RParen) {
            // Empty list
            GetToken();
            return LispMemory::Nil;
        }
        auto car = Read();
        if (token_ == LispTokenType::RParen) {
            GetToken();
            return mem_.Cons(car, LispMemory::Nil);
        }
        else if (token_ == LispTokenType::Dot) {
            GetToken();
            return mem_.Cons(car, Read());
        }
        else {
            return mem_.Cons(car, ReadCons());
        }
    }

}
