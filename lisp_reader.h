#pragma once

#include "lisp_memory.h"
#include <string>
#include <sstream>

namespace procdraw {

    class LispInterpreter;

    enum class LispTokenType {
        SingleQuote, LParen, RParen, Dot,
        Number, Symbol, String, EndOfInput, Undefined
    };

    class LispReader {
    public:
        LispObjectPtr Read(LispInterpreter *L, const std::string &str);
    private:
        std::istringstream input_;
        int ch_;
        LispTokenType token_;
        double numVal_;
        std::string symbolVal_;
        std::string stringVal_;
        void GetCh();
        void GetToken();
        bool IsStartOfNumber();
        void GetNumber();
        void Expect(LispTokenType t);
        LispObjectPtr Read(LispInterpreter *L);
        LispObjectPtr ReadCons(LispInterpreter *L);
    };

}
