#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"
#include "lisp_printer.h"
#include <string>
#include <vector>

namespace procdraw {

    enum class LispObjectType {
        Null, Number, Symbol, Cons, CFunction, Boolean, String, Table, Eof
    };

    typedef LispObjectPtr(*lisp_CFunction) (LispInterpreter *L, LispObjectPtr args,
                                            LispObjectPtr env, void *data);

    class LispInterpreter {
    public:
        LispInterpreter();
        // Symbol literals
        LispObjectPtr Nil;
        LispObjectPtr True;
        LispObjectPtr False;
        LispObjectPtr Eof;
        // Memory allocation
        LispObjectPtr MakeNumber(double val);
        LispObjectPtr Cons(LispObjectPtr car, LispObjectPtr cdr);
        LispObjectPtr MakeCFunction(lisp_CFunction cfun, void *data);
        LispObjectPtr MakeString(const std::string &str);
        LispObjectPtr MakeTable();
        // Memory access
        LispObjectType TypeOf(LispObjectPtr obj);
        bool Null(LispObjectPtr obj);
        bool IsEof(LispObjectPtr obj);
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
        LispObjectPtr Keys(LispObjectPtr table);
        LispObjectPtr Clear(LispObjectPtr table);
        // Symbols
        LispObjectPtr SymbolRef(const std::string &name);
        LispObjectPtr SetGlobalCFunction(const std::string &name, lisp_CFunction cfun, void *data);
        // Functions
        LispObjectPtr Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr ApplyTableMethod(LispObjectPtr key, LispObjectPtr table, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Assoc(LispObjectPtr key, LispObjectPtr alist);
        bool Atom(LispObjectPtr obj);
        LispObjectPtr Caar(LispObjectPtr obj);
        LispObjectPtr Cadr(LispObjectPtr obj);
        LispObjectPtr Cdar(LispObjectPtr obj);
        LispObjectPtr Cddr(LispObjectPtr obj);
        LispObjectPtr Caddr(LispObjectPtr obj);
        LispObjectPtr Cadddr(LispObjectPtr obj);
        LispObjectPtr Caddddr(LispObjectPtr obj);
        LispObjectPtr Call(const std::string &name);
        BalancedState CheckBalanced(const std::string &str);
        LispObjectPtr Eval(LispObjectPtr exp, LispObjectPtr env);
        LispObjectPtr Eval(LispObjectPtr exp);
        LispObjectPtr Evif(LispObjectPtr arglist, LispObjectPtr env);
        LispObjectPtr Evlis(LispObjectPtr arglist, LispObjectPtr env);
        LispObjectPtr MakeList(std::vector<LispObjectPtr> objs);
        std::string PrintToString(LispObjectPtr obj);
        LispObjectPtr Progn(LispObjectPtr actions, LispObjectPtr env);
        LispObjectPtr Putassoc(LispObjectPtr key, LispObjectPtr val, LispObjectPtr alist);
        LispObjectPtr Read(const std::string &str);
        LispObjectPtr Set(LispObjectPtr symbol, LispObjectPtr value, LispObjectPtr env);
        LispObjectPtr Value(LispObjectPtr symbol, LispObjectPtr env);
    private:
        LispObjectPtr symbols_;
        LispReader reader_;
        LispPrinter printer_;
        LispObjectPtr S_DEF;
        LispObjectPtr S_IF;
        LispObjectPtr S_LAMBDA;
        LispObjectPtr S_PROGN;
        LispObjectPtr S_QUOTE;
        LispObjectPtr S_SETQ;
        void InitSymbolLiterals();
        LispObjectPtr ApplyCFunction(LispObjectPtr cfun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Assoc1(LispObjectPtr key, LispObjectPtr alist, bool *found);
        LispObjectPtr Bind(LispObjectPtr vars, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr MakeSymbol(const std::string &name, LispObjectPtr value);
        LispObjectPtr SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value);
    };

}
