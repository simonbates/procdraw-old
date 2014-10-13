#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"

namespace procdraw {

    class LispInterpreter {
    public:
        bool Atom(LispObjectPtr obj);
        LispObjectPtr Read(const std::string &str);
        std::string PrintString(LispObjectPtr obj);
    private:
        LispMemory mem_;
        LispReader reader_{mem_};
    };

}
