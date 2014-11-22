#pragma once

#include "lisp_memory.h"
#include <string>
#include <sstream>

namespace procdraw {

    class LispInterpreter;

    enum class LispTokenType { LParen, RParen, Dot, Plus, Minus, Star, Slash, Number, Symbol, EndOfInput, Undefined };

    class LispReader {
    public:
        LispObjectPtr Read(LispInterpreter *L, const std::string &str);
    private:
        std::istringstream input_;
        int ch_;
        LispTokenType token_;
        double numVal_;
        std::string symbolVal_;
        void GetCh();
        void GetToken();
        bool IsStartOfNumber();
        void GetNumber();
        LispObjectPtr Read(LispInterpreter *L);
        LispObjectPtr ReadCons(LispInterpreter *L);
    };

}
