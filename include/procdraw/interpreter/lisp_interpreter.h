#pragma once

#include "lisp_memory.h"
#include "lisp_reader.h"
#include "lisp_printer.h"
#include "prettyprinter.h"
#include <string>
#include <vector>

namespace procdraw {

    enum class LispObjectType {
        Null, Number, Symbol, Cons, CFunction, Boolean, String, Dictionary, Eof
    };

    typedef LispObjectPtr(*lisp_CFunction) (LispInterpreter *L, LispObjectPtr args,
                                            LispObjectPtr env, void *data);

    class LispInterpreter {
    public:
        LispInterpreter();
        // Singletons
        LispObjectPtr Nil;
        LispObjectPtr True;
        LispObjectPtr False;
        LispObjectPtr Eof;
        // Memory allocation
        LispObjectPtr MakeNumber(double val);
        LispObjectPtr Cons(LispObjectPtr car, LispObjectPtr cdr);
        LispObjectPtr MakeCFunction(lisp_CFunction cfun, void *data);
        LispObjectPtr MakeString(const std::string &str);
        LispObjectPtr MakeDict();
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
        LispObjectPtr Rplaca(LispObjectPtr cons, LispObjectPtr obj);
        LispObjectPtr Rplacd(LispObjectPtr cons, LispObjectPtr obj);
        LispObjectPtr Get(LispObjectPtr key, LispObjectPtr dict, LispObjectPtr notFound);
        LispObjectPtr Get(LispObjectPtr key, LispObjectPtr dict);
        LispObjectPtr Put(LispObjectPtr key, LispObjectPtr val, LispObjectPtr dict);
        LispObjectPtr Keys(LispObjectPtr dict);
        LispObjectPtr Clear(LispObjectPtr dict);
        // Symbols
        LispObjectPtr SymbolRef(const std::string &name);
        LispObjectPtr SetGlobalCFunction(const std::string &name, lisp_CFunction cfun, void *data);
        // Functions
        LispObjectPtr Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Assoc(LispObjectPtr key, LispObjectPtr alist);
        bool Atom(LispObjectPtr obj);
        LispObjectPtr BoolToLisp(bool b);
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
        bool Functionp(LispObjectPtr obj);
        LispObjectPtr MakeList(std::vector<LispObjectPtr> objs);
        bool Memb(LispObjectPtr obj, LispObjectPtr list);
        LispObjectPtr Not(LispObjectPtr obj);
        std::string PrettyPrintToString(LispObjectPtr obj, int margin);
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
        PrettyPrinter prettyPrinter_;
        LispObjectPtr S_DEF;
        LispObjectPtr S_IF;
        LispObjectPtr S_LAMBDA;
        LispObjectPtr S_PROGN;
        LispObjectPtr S_QUOTE;
        LispObjectPtr S_SETQ;
        void InitSingletons();
        LispObjectPtr ApplyCFunction(LispObjectPtr cfun, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr Assoc1(LispObjectPtr key, LispObjectPtr alist, bool *found);
        LispObjectPtr Bind(LispObjectPtr vars, LispObjectPtr args, LispObjectPtr env);
        LispObjectPtr MakeSymbol(const std::string &name, LispObjectPtr value);
        LispObjectPtr SetSymbolValue(LispObjectPtr symbol, LispObjectPtr value);
    };

}
