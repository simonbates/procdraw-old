#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"
#include <string>

namespace procdraw {

    enum class LispObjectType { Nil, Number, Symbol, Cons };

    class LispInterpreter {
    public:
        LispInterpreter();
        // Nil
        LispObjectPtr Nil;
        // Memory allocation
        LispObjectPtr MakeNumber(double val);
        LispObjectPtr MakeSymbol(const std::string &str);
        LispObjectPtr Cons(LispObjectPtr car, LispObjectPtr cdr);
        // Memory access
        LispObjectType TypeOf(LispObjectPtr obj);
        bool Null(LispObjectPtr obj);
        double NumVal(LispObjectPtr obj);
        std::string StringVal(LispObjectPtr obj);
        LispObjectPtr Car(LispObjectPtr obj);
        LispObjectPtr Cdr(LispObjectPtr obj);
        // Functions
        bool Atom(LispObjectPtr obj);
        LispObjectPtr Read(const std::string &str);
        std::string PrintString(LispObjectPtr obj);
    private:
        LispObjectPtr symbolTable_;
        LispReader reader_;
        void InitNil();
    };

}
