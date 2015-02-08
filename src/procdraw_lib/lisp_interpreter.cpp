#include "lisp_interpreter.h"
#include "lisp_functions.h"
#include <cmath>

namespace procdraw {

    LispInterpreter::LispInterpreter()
    {
        InitSymbolLiterals();
        symbols_ = Nil;
        // Special forms
        S_IF = SymbolRef("if");
        S_LAMBDA = SymbolRef("lambda");
        S_PROGN = SymbolRef("progn");
        S_QUOTE = SymbolRef("quote");
        S_SETQ = SymbolRef("setq");
        // Functions
        SetGlobalCFunction("*", lisp_Product);
        SetGlobalCFunction("+", lisp_Sum);
        SetGlobalCFunction("-", lisp_Difference);
        SetGlobalCFunction("/", lisp_Quotient);
        SetGlobalCFunction("apply", lisp_Apply);
        SetGlobalCFunction("car", lisp_Car);
        SetGlobalCFunction("cdr", lisp_Cdr);
        SetGlobalCFunction("eq", lisp_Eq);
        SetGlobalCFunction("get", lisp_Get);
        SetGlobalCFunction("make-table", lisp_MakeTable);
        SetGlobalCFunction("map-range", lisp_MapRange);
        SetGlobalCFunction("put", lisp_Put);
        SetGlobalCFunction("wrap-range", lisp_WrapRange);
        // Constants
        Set(SymbolRef("pi"), MakeNumber(M_PI), Nil);
    }

    LispObjectPtr LispInterpreter::SymbolRef(const std::string &name)
    {
        LispObjectPtr n = symbols_;
        while (!Null(n)) {
            auto symbol = Car(n);
            if (SymbolName(symbol) == name) {
                return symbol;
            }
            n = Cdr(n);
        }
        auto symbol = MakeSymbol(name, Nil);
        symbols_ = Cons(symbol, symbols_);
        return symbol;
    }

    LispObjectPtr LispInterpreter::SetGlobalCFunction(const std::string &name, lisp_CFunction cfun)
    {
        return Set(SymbolRef(name), MakeCFunction(cfun), Nil);
    }

    LispObjectPtr LispInterpreter::Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env)
    {
        switch (TypeOf(fun)) {
        case LispObjectType::CFunction:
            return ApplyCFunction(fun, args, env);
        case LispObjectType::Cons:
            return Eval(Caddr(fun), Bind(Cadr(fun), args, env));
        case LispObjectType::Symbol:
            return Apply(Get(Car(args), fun), args, env);
        default:
            // TODO complain
            return Nil;
        }
    }

    LispObjectPtr LispInterpreter::Apply(const std::string &name)
    {
        return Apply(Value(SymbolRef(name), Nil), Nil, Nil);
    }

    LispObjectPtr LispInterpreter::Assoc(LispObjectPtr key, LispObjectPtr alist)
    {
        LispObjectPtr n = alist;
        while (!Null(n)) {
            auto pair = Car(n);
            if (Eq(key, Car(pair))) {
                return pair;
            }
            n = Cdr(n);
        }
        return Nil;
    }

    bool LispInterpreter::Atom(LispObjectPtr obj)
    {
        return TypeOf(obj) != LispObjectType::Cons;
    }

    LispObjectPtr LispInterpreter::Bind(LispObjectPtr vars, LispObjectPtr args, LispObjectPtr env)
    {
        // TODO verify that vars and args are the same length
        auto v = vars;
        auto a = args;
        while (!Null(v)) {
            env = Cons(Cons(Car(v), Car(a)), env);
            v = Cdr(v);
            a = Cdr(a);
        }
        return env;
    }

    LispObjectPtr LispInterpreter::Caar(LispObjectPtr obj)
    {
        return Car(Car(obj));
    }

    LispObjectPtr LispInterpreter::Cadr(LispObjectPtr obj)
    {
        return Car(Cdr(obj));
    }

    LispObjectPtr LispInterpreter::Cdar(LispObjectPtr obj)
    {
        return Cdr(Car(obj));
    }

    LispObjectPtr LispInterpreter::Cddr(LispObjectPtr obj)
    {
        return Cdr(Cdr(obj));
    }

    LispObjectPtr LispInterpreter::Caddr(LispObjectPtr obj)
    {
        return Car(Cdr(Cdr(obj)));
    }

    LispObjectPtr LispInterpreter::Cadddr(LispObjectPtr obj)
    {
        return Car(Cdr(Cdr(Cdr(obj))));
    }

    LispObjectPtr LispInterpreter::Caddddr(LispObjectPtr obj)
    {
        return Car(Cdr(Cdr(Cdr(Cdr(obj)))));
    }

    LispObjectPtr LispInterpreter::Eval(LispObjectPtr exp, LispObjectPtr env)
    {
        if (Atom(exp)) {
            switch (TypeOf(exp)) {
            case LispObjectType::Boolean:
            case LispObjectType::Nil:
            case LispObjectType::Number:
            case LispObjectType::String:
                return exp;
            default:
                return Value(exp, env);
            }
        }
        else {
            auto first = Car(exp);
            if (Eq(first, S_IF)) {
                return Evif(Cdr(exp), env);
            }
            else if (Eq(first, S_LAMBDA)) {
                return exp;
            }
            else if (Eq(first, S_PROGN)) {
                return Progn(Cdr(exp), env);
            }
            else if (Eq(first, S_QUOTE)) {
                return Cadr(exp);
            }
            else if (Eq(first, S_SETQ)) {
                return Set(Cadr(exp), Eval(Caddr(exp), env), env);
            }
            else {
                return Apply(Eval(first, env), Evlis(Cdr(exp), env), env);
            }
        }

        return Nil;
    }

    LispObjectPtr LispInterpreter::Eval(LispObjectPtr exp)
    {
        return Eval(exp, Nil);
    }

    LispObjectPtr LispInterpreter::Evif(LispObjectPtr arglist, LispObjectPtr env) {
        if (BoolVal(Eval(Car(arglist), env))) {
            return Eval(Cadr(arglist), env);
        }
        else {
            return Eval(Caddr(arglist), env);
        }
    }

    LispObjectPtr LispInterpreter::Evlis(LispObjectPtr arglist, LispObjectPtr env) {
        if (Null(arglist)) {
            return Nil;
        }
        else {
            return Cons(Eval(Car(arglist), env), Evlis(Cdr(arglist), env));
        }
    }

    LispObjectPtr LispInterpreter::MakeList(std::vector<LispObjectPtr> objs)
    {
        auto list1 = Nil;
        for (auto it = objs.rbegin(); it != objs.rend(); ++it) {
            list1 = Cons(*it, list1);
        }
        return list1;
    }

    std::string LispInterpreter::PrintString(LispObjectPtr obj)
    {
        switch (TypeOf(obj)) {
        case LispObjectType::Nil:
            return "nil";
        case LispObjectType::Number:
        {
            std::ostringstream s;
            s << NumVal(obj);
            return s.str();
        }
        case LispObjectType::Symbol:
            return SymbolName(obj);
        case LispObjectType::Cons:
        {
            if (Eq(Car(obj), S_QUOTE)) {
                return "'" + PrintString(Cadr(obj));
            }
            else {
                std::string s("(");
                LispObjectPtr n = obj;
                while (!Null(n)) {
                    s.append(PrintString(Car(n)));
                    n = Cdr(n);
                    if (!Null(n) && Atom(n)) {
                        s.append(" . ");
                        s.append(PrintString(n));
                        n = LispInterpreter::Nil;
                    }
                    if (!Null(n)) {
                        s.append(" ");
                    }
                }
                s.append(")");
                return s;
            }
        }
        case LispObjectType::CFunction:
            return "<CFunction>";
        case LispObjectType::Boolean:
            return BoolVal(obj) ? "true" : "false";
        case LispObjectType::String:
            return "\"" + StringVal(obj) + "\"";
        case LispObjectType::Table:
            // TODO Table serialization
            return "<Table>";
        default:
            return "";
        }
    }

    LispObjectPtr LispInterpreter::Progn(LispObjectPtr actions, LispObjectPtr env)
    {
        LispObjectPtr result = Nil;
        LispObjectPtr n = actions;
        while (!Null(n)) {
            result = Eval(Car(n), env);
            n = Cdr(n);
        }
        return result;
    }

    LispObjectPtr LispInterpreter::Read(const std::string &str)
    {
        return reader_.Read(this, str);
    }

    LispObjectPtr LispInterpreter::Set(LispObjectPtr symbol, LispObjectPtr value, LispObjectPtr env)
    {
        auto binding = Assoc(symbol, env);
        if (Null(binding)) {
            SetSymbolValue(symbol, value);
        }
        else {
            Rplacd(binding, value);
        }
        return value;
    }

    LispObjectPtr LispInterpreter::Value(LispObjectPtr symbol, LispObjectPtr env)
    {
        auto binding = Assoc(symbol, env);
        if (Null(binding)) {
            return SymbolValue(symbol);
        }
        else {
            return Cdr(binding);
        }
    }

}
