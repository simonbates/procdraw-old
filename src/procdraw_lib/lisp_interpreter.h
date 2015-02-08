#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"
#include <string>
#include <vector>

namespace procdraw {

    enum class LispObjectType { Nil, Number, Symbol, Cons, CFunction, Boolean, String, Table };

    typedef LispObjectPtr(*lisp_CFunction) (LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

    class LispInterpreter {
    public:
        LispInterpreter();
        // Symbol literals
        LispObjectPtr Nil;
        LispObjectPtr True;
        LispObjectPtr False;
        // Memory allocation
        LispObjectPtr MakeNumber(double val);
        LispObjectPtr Cons(LispObjectPtr car, LispObjectPtr cdr);
        LispObjectPtr MakeString(const std::string &str);
        LispObjectPtr MakeTable();
        // Memory access
        LispObjectType TypeOf(LispObjectPtr obj);
        bool Null(LispObjectPtr obj);
        double NumVal(LispObjectPtr obj);
        std::string SymbolName(LispObjectPtr symbol);
        LispObjectPtr SymbolValue(LispObjectPtr symbol);
        LispObjectPtr Car(LispObjectPtr obj);
        LispObjectPtr Cdr(LispObjectPtr obj);
        bool BoolVal(LispObjectPtr obj);
        std::string StringVal(LispObjectPtr obj);
        bool Eq(LispObjectPtr x, LispObjectPtr y);
        LispObjectPtr Rplaca(LispObjectPtr cons, LispObjectPtr obj);
        LispObjectPtr Rplacd(LispObjectPtr cons, LispObjectPtr obj);
        LispObjectPtr Get(LispObjectPtr table, LispObjectPtr key);
        LispObjectPtr Put(LispObjectPtr table, LispObjectPtr key, LispObjectPtr val);
        // Symbols
        LispObjectPtr SymbolRef(const std::string &name);
        LispObjectPtr SetGlobalCFunction(const std::string &name, lisp_CFunction cfun);
        // Functions
        LispObjectPtr Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Apply(const std::string &name);
        LispObjectPtr Assoc(LispObjectPtr key, LispObjectPtr alist);
        bool Atom(LispObjectPtr obj);
        LispObjectPtr Caar(LispObjectPtr obj);
        LispObjectPtr Cadr(LispObjectPtr obj);
        LispObjectPtr Cdar(LispObjectPtr obj);
        LispObjectPtr Cddr(LispObjectPtr obj);
        LispObjectPtr Caddr(LispObjectPtr obj);
        LispObjectPtr Cadddr(LispObjectPtr obj);
        LispObjectPtr Caddddr(LispObjectPtr obj);
        LispObjectPtr Eval(LispObjectPtr exp, LispObjectPtr env);
        LispObjectPtr Eval(LispObjectPtr exp);
        LispObjectPtr Evif(LispObjectPtr arglist, LispObjectPtr env);
        LispObjectPtr Evlis(LispObjectPtr arglist, LispObjectPtr env);
        LispObjectPtr MakeList(std::vector<LispObjectPtr> objs);
        std::string PrintString(LispObjectPtr obj);
        LispObjectPtr Progn(LispObjectPtr actions, LispObjectPtr env);
        LispObjectPtr Read(const std::string &str);
        LispObjectPtr Set(LispObjectPtr symbol, LispObjectPtr value, LispObjectPtr env);
        LispObjectPtr Value(LispObjectPtr symbol, LispObjectPtr env);
    private:
        LispObjectPtr symbols_;
        LispReader reader_;
        LispObjectPtr S_IF;
        LispObjectPtr S_LAMBDA;
        LispObjectPtr S_PROGN;
        LispObjectPtr S_QUOTE;
        LispObjectPtr S_SETQ;
        void InitSymbolLiterals();
        LispObjectPtr ApplyCFunction(LispObjectPtr cfun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Bind(LispObjectPtr vars, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr MakeCFunction(lisp_CFunction cfun);
        LispObjectPtr MakeSymbol(const std::string &name, LispObjectPtr value);
        LispObjectPtr SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value);
    };

}
