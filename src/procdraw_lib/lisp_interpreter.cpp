#include "lisp_interpreter.h"
#include "lisp_functions.h"
#include <cmath>

namespace procdraw {

    LispInterpreter::LispInterpreter()
    {
        InitNil();
        symbols_ = Nil;
        // Special forms
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
        if (TypeOf(fun) == LispObjectType::CFunction) {
            return ApplyCFunction(fun, args, env);
        }
        else {
            return Eval(Caddr(fun), Bind(Cadr(fun), args, env));
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

    LispObjectPtr LispInterpreter::Cadr(LispObjectPtr obj)
    {
        return Car(Cdr(obj));
    }

    LispObjectPtr LispInterpreter::Caddr(LispObjectPtr obj)
    {
        return Car(Cdr(Cdr(obj)));
    }

    LispObjectPtr LispInterpreter::Eval(LispObjectPtr exp, LispObjectPtr env)
    {
        if (Atom(exp)) {
            if (TypeOf(exp) == LispObjectType::Number) {
                return exp;
            }
            else {
                return Value(exp, env);
            }
        }
        else {
            auto first = Car(exp);
            if (Eq(first, S_QUOTE)) {
                return Cadr(exp);
            }
            else if (Eq(first, S_LAMBDA)) {
                return exp;
            }
            else if (Eq(first, S_SETQ)) {
                return Set(Cadr(exp), Eval(Caddr(exp), env), env);
            }
            else if (Eq(first, S_PROGN)) {
                return Progn(Cdr(exp), env);
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

    LispObjectPtr LispInterpreter::Evlis(LispObjectPtr arglist, LispObjectPtr env) {
        if (Null(arglist)) {
            return Nil;
        }
        else {
            return Cons(Eval(Car(arglist), env), Evlis(Cdr(arglist), env));
        }
    }

    LispObjectPtr LispInterpreter::List(std::vector<LispObjectPtr> objs)
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
        case LispObjectType::CFunction:
            return "<CFunction>";
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
