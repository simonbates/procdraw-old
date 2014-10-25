#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"
#include <string>
#include <vector>

namespace procdraw {

    enum class LispObjectType { Nil, Number, Symbol, Cons };

    class LispInterpreter {
    public:
        LispInterpreter();
        // Nil
        LispObjectPtr Nil;
        // Memory allocation
        LispObjectPtr MakeNumber(double val);
        LispObjectPtr Cons(LispObjectPtr car, LispObjectPtr cdr);
        // Memory access
        LispObjectType TypeOf(LispObjectPtr obj);
        bool Null(LispObjectPtr obj);
        double NumVal(LispObjectPtr obj);
        std::string StringVal(LispObjectPtr obj);
        LispObjectPtr Car(LispObjectPtr obj);
        LispObjectPtr Cdr(LispObjectPtr obj);
        bool Eq(LispObjectPtr x, LispObjectPtr y);
        LispObjectPtr Rplaca(LispObjectPtr cons, LispObjectPtr obj);
        LispObjectPtr Rplacd(LispObjectPtr cons, LispObjectPtr obj);
        // Symbols
        LispObjectPtr SymbolRef(const std::string &str);
        // Functions
        LispObjectPtr Add(LispObjectPtr args);
        LispObjectPtr Assoc(LispObjectPtr key, LispObjectPtr alist);
        bool Atom(LispObjectPtr obj);
        LispObjectPtr Eval(LispObjectPtr exp, LispObjectPtr env);
        LispObjectPtr Evlis(LispObjectPtr arglist, LispObjectPtr env);
        LispObjectPtr List(std::vector<LispObjectPtr> objs);
        LispObjectPtr Read(const std::string &str);
        std::string PrintString(LispObjectPtr obj);
    private:
        LispObjectPtr symbols_;
        LispReader reader_;
        LispObjectPtr S_ADD;
        LispObjectPtr S_QUOTE;
        void InitNil();
        LispObjectPtr MakeSymbol(const std::string &str);
    };

}
