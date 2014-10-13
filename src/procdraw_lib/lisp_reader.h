#pragma once

#include "lisp_memory.h"
#include <memory>
#include <sstream>
#include <string>

namespace procdraw {

    enum class LispTokenType { LParen, RParen, Dot, Number, Symbol, EndOfInput, Undefined };

    class LispReader {
    public:
        LispReader(LispMemory &mem) : mem_(mem) { }
        LispObjectPtr Read(const std::string &str);
    private:
        LispMemory &mem_;
        std::istringstream input_;
        int ch_;
        LispTokenType token_;
        double numVal_;
        std::string symbolVal_;
        void GetCh();
        void GetToken();
        LispObjectPtr Read();
        LispObjectPtr ReadCons();
    };

}
