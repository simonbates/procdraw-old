#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"
#include <string>
#include <vector>

namespace procdraw {

    enum class LispObjectType { Nil, Number, Symbol, Cons, CFunction };

    typedef LispObjectPtr(*lisp_CFunction) (LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

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
        std::string SymbolName(LispObjectPtr symbol);
        LispObjectPtr SymbolValue(LispObjectPtr symbol);
        LispObjectPtr Car(LispObjectPtr obj);
        LispObjectPtr Cdr(LispObjectPtr obj);
        bool Eq(LispObjectPtr x, LispObjectPtr y);
        LispObjectPtr Rplaca(LispObjectPtr cons, LispObjectPtr obj);
        LispObjectPtr Rplacd(LispObjectPtr cons, LispObjectPtr obj);
        // Symbols
        LispObjectPtr SymbolRef(const std::string &name);
        LispObjectPtr SetGlobalCFunction(const std::string &name, lisp_CFunction cfun);
        // Functions
        LispObjectPtr Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Assoc(LispObjectPtr key, LispObjectPtr alist);
        bool Atom(LispObjectPtr obj);
        LispObjectPtr Cadr(LispObjectPtr obj);
        LispObjectPtr Caddr(LispObjectPtr obj);
        LispObjectPtr Eval(LispObjectPtr exp, LispObjectPtr env);
        LispObjectPtr Eval(LispObjectPtr exp);
        LispObjectPtr Evlis(LispObjectPtr arglist, LispObjectPtr env);
        LispObjectPtr List(std::vector<LispObjectPtr> objs);
        std::string PrintString(LispObjectPtr obj);
        LispObjectPtr Progn(LispObjectPtr actions, LispObjectPtr env);
        LispObjectPtr Read(const std::string &str);
        LispObjectPtr Set(LispObjectPtr symbol, LispObjectPtr value, LispObjectPtr env);
        LispObjectPtr Value(LispObjectPtr symbol, LispObjectPtr env);
    private:
        LispObjectPtr symbols_;
        LispReader reader_;
        LispObjectPtr S_LAMBDA;
        LispObjectPtr S_PROGN;
        LispObjectPtr S_QUOTE;
        LispObjectPtr S_SETQ;
        void InitNil();
        LispObjectPtr ApplyCFunction(LispObjectPtr cfun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Bind(LispObjectPtr vars, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr MakeCFunction(lisp_CFunction cfun);
        LispObjectPtr MakeSymbol(const std::string &name, LispObjectPtr value);
        LispObjectPtr SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value);
    };

}
