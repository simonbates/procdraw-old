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

        switch (ch_) {
        case EOF:
            token_ = LispTokenType::EndOfInput;
            break;
        case '"': {
            std::string str;
            GetCh();
            while (true) {
                if (ch_ == '"') {
                    break;
                }
                if (ch_ == EOF) {
                    throw std::runtime_error("Non-closed string at LispReader::Read()");
                }
                str += ch_;
                GetCh();
            }
            // move past the closing '"'
            GetCh();
            token_ = LispTokenType::String;
            stringVal_ = str;
            break;
        }
        case '\'':
            token_ = LispTokenType::SingleQuote;
            GetCh();
            break;
        case '(':
            token_ = LispTokenType::LParen;
            GetCh();
            break;
        case ')':
            token_ = LispTokenType::RParen;
            GetCh();
            break;
        case '*':
            token_ = LispTokenType::Star;
            GetCh();
            break;
        case '+':
            GetCh();
            if (IsStartOfNumber()) {
                GetNumber();
            }
            else {
                token_ = LispTokenType::Plus;
            }
            break;
        case '-':
            GetCh();
            if (IsStartOfNumber()) {
                GetNumber();
                numVal_ = -numVal_;
            }
            else {
                token_ = LispTokenType::HyphenMinus;
            }
            break;
        case '.':
            token_ = LispTokenType::Dot;
            GetCh();
            break;
        case '/':
            token_ = LispTokenType::Slash;
            GetCh();
            break;
        default:
            if (IsStartOfNumber()) {
                GetNumber();
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
    }

    bool LispReader::IsStartOfNumber()
    {
        return isdigit(ch_);
    }

    void LispReader::GetNumber()
    {
        std::string number;
        while (isdigit(ch_)) {
            number += ch_;
            GetCh();
        }
        token_ = LispTokenType::Number;
        numVal_ = atoi(number.c_str());
    }

    LispObjectPtr LispReader::Read(LispInterpreter *L)
    {
        switch (token_) {
        case LispTokenType::SingleQuote:
            GetToken();
            return L->Cons(L->SymbolRef("quote"), L->Cons(Read(L), L->Nil));
        case LispTokenType::LParen:
            GetToken();
            return ReadCons(L);
        case LispTokenType::Number: {
            auto intObj = L->MakeNumber(numVal_);
            GetToken();
            return intObj;
        }
        case LispTokenType::Symbol: {
            LispObjectPtr obj;
            if (symbolVal_ == "nil") {
                obj = L->Nil;
            }
            else if (symbolVal_ == "true") {
                obj = L->True;
            }
            else if (symbolVal_ == "false") {
                obj = L->False;
            }
            else {
                obj = L->SymbolRef(symbolVal_);
            }
            GetToken();
            return obj;
        }
        case LispTokenType::String: {
            auto strObj = L->MakeString(stringVal_);
            GetToken();
            return strObj;
        }
        case LispTokenType::Star:
            GetToken();
            return L->SymbolRef("*");
        case LispTokenType::Plus:
            GetToken();
            return L->SymbolRef("+");
        case LispTokenType::HyphenMinus:
            GetToken();
            return L->SymbolRef("-");
        case LispTokenType::Slash:
            GetToken();
            return L->SymbolRef("/");
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
            // TODO I am recursing for each list item -- instead iterate to reduce stack usage?
            return L->Cons(car, ReadCons(L));
        }
    }

}
