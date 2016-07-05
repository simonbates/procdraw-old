#include "lisp_reader.h"
#include "lisp_interpreter.h"
#include <cctype>

namespace procdraw {

    LispObjectPtr LispReader::Read(LispInterpreter *L, const std::string &str)
    {
        SetInput(str);
        return Read(L);
    }

    BalancedState LispReader::CheckBalanced(const std::string &str)
    {
        SetInput(str);
        int parenCount = 0;

        while (token_ != LispTokenType::EndOfInput
                && token_ != LispTokenType::NonClosedString) {
            if (token_ == LispTokenType::LParen) {
                ++parenCount;
            }
            else if (token_ == LispTokenType::RParen) {
                --parenCount;
            }
            GetToken();
        };

        if (token_ == LispTokenType::NonClosedString) {
            return BalancedState::NotClosed;
        }
        else if (parenCount > 0) {
            return BalancedState::NotClosed;
        }
        else if (parenCount < 0) {
            return BalancedState::TooManyClosingParens;
        }
        return BalancedState::Balanced;
    }

    void LispReader::SetInput(const std::string &str)
    {
        input_.str(str);
        input_.clear();
        GetCh();
        GetToken();
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
        case '"': {
            GetString();
            break;
        }
        case'$':
            token_ = LispTokenType::Dollar;
            GetCh();
            break;
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
        case '+':
            GetCh();
            if (IsStartOfNumber()) {
                GetNumber();
            }
            else {
                token_ = LispTokenType::Symbol;
                symbolVal_ = "+";
            }
            break;
        case '-':
            GetCh();
            if (IsStartOfNumber()) {
                GetNumber();
                numVal_ = -numVal_;
            }
            else {
                token_ = LispTokenType::Symbol;
                symbolVal_ = "-";
            }
            break;
        case '.':
            token_ = LispTokenType::Dot;
            GetCh();
            break;
        case '=':
            GetCh();
            if (ch_ == '>') {
                token_ = LispTokenType::Symbol;
                symbolVal_ = "=>";
                GetCh();
            }
            else {
                token_ = LispTokenType::Undefined;
            }
            break;
        // Single char symbols
        case '*':
        case '/':
            token_ = LispTokenType::Symbol;
            symbolVal_ = std::string(1, ch_);
            GetCh();
            break;
        case EOF:
            token_ = LispTokenType::EndOfInput;
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
                GetCh();
            }
            break;
        }
    }

    void LispReader::GetString()
    {
        std::string str;
        // Consume the opening '"'
        GetCh();
        while (ch_ != '"') {
            if (ch_ == EOF) {
                token_ = LispTokenType::NonClosedString;
                return;
            }
            str += ch_;
            GetCh();
        }
        // Consume the closing '"'
        GetCh();
        token_ = LispTokenType::String;
        stringVal_ = str;
    }

    bool LispReader::IsStartOfNumber()
    {
        return isdigit(ch_) != 0;
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

    void LispReader::Expect(LispTokenType t)
    {
        if (token_ == t) {
            GetToken();
        }
        else {
            // TODO custom syntax error type
            throw std::runtime_error("Syntax error");
        }
    }

    LispObjectPtr LispReader::Read(LispInterpreter *L)
    {
        switch (token_) {
        case LispTokenType::Dollar:
            GetToken();
            return L->Cons(L->SymbolRef("sigval"), L->Cons(Read(L), L->Nil));
        case LispTokenType::SingleQuote:
            GetToken();
            return L->Cons(L->SymbolRef("quote"), L->Cons(Read(L), L->Nil));
        case LispTokenType::LParen:
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
        case LispTokenType::EndOfInput:
            return L->Eof;
        default:
            // TODO custom syntax error type
            throw std::runtime_error("Syntax error");
        }
    }

    LispObjectPtr LispReader::ReadCons(LispInterpreter *L)
    {
        Expect(LispTokenType::LParen);

        if (token_ == LispTokenType::RParen) {
            GetToken();
            // Empty list
            return L->Nil;
        }

        auto head = L->Cons(Read(L), L->Nil);
        auto prev = head;

        while (token_ != LispTokenType::EndOfInput) {
            if (token_ == LispTokenType::RParen) {
                GetToken();
                return head;
            }
            else if (token_ == LispTokenType::Dot) {
                GetToken();
                if (token_ == LispTokenType::EndOfInput) {
                    return L->Eof;
                }
                auto cdr = Read(L);
                Expect(LispTokenType::RParen);
                L->Rplacd(prev, cdr);
                return head;
            }
            auto next = L->Cons(Read(L), L->Nil);
            L->Rplacd(prev, next);
            prev = next;
        }

        return L->Eof;
    }

}
