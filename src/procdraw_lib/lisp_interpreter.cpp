#include "lisp_interpreter.h"

namespace procdraw {

    LispInterpreter::LispInterpreter()
    {
        InitNil();
        symbols_ = Nil;
        globalEnv_ = Nil;
        S_ADD = SymbolRef("add");
        S_APPLY = SymbolRef("apply");
        S_LAMBDA = SymbolRef("lambda");
        S_QUOTE = SymbolRef("quote");
    }

    LispObjectPtr LispInterpreter::SymbolRef(const std::string &str)
    {
        LispObjectPtr n = symbols_;
        while (!Null(n)) {
            auto symbol = Car(n);
            if (StringVal(symbol) == str) {
                return symbol;
            }
            n = Cdr(n);
        }
        auto symbol = MakeSymbol(str);
        symbols_ = Cons(symbol, symbols_);
        return symbol;
    }

    LispObjectPtr LispInterpreter::Add(LispObjectPtr args)
    {
        double sum = 0;
        LispObjectPtr n = args;
        while (!Null(n)) {
            sum += NumVal(Car(n));
            n = Cdr(n);
        }
        return MakeNumber(sum);
    }

    LispObjectPtr LispInterpreter::Apply(LispObjectPtr fun, LispObjectPtr args, LispObjectPtr env)
    {
        return Eval(Caddr(fun), Bind(Cadr(fun), args, env));
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
                return Cdr(Assoc(exp, env));
            }
        }
        else {
            if (Eq(Car(exp), S_QUOTE)) {
                return Car(Cdr(exp));
            }
            else if (Eq(Car(exp), S_LAMBDA)) {
                return exp;
            }
            else if (Eq(Car(exp), S_APPLY)) {
                return Apply(Eval(Cadr(exp), env), Eval(Caddr(exp), env), env);
            }
            else if (Eq(Car(exp), S_ADD)) {
                return Add(Evlis(Cdr(exp), env));
            }
        }

        return Nil;
    }

    LispObjectPtr LispInterpreter::Eval(LispObjectPtr exp)
    {
        return Eval(exp, globalEnv_);
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

    LispObjectPtr LispInterpreter::Read(const std::string &str)
    {
        return reader_.Read(this, str);
    }

    std::string LispInterpreter::PrintString(LispObjectPtr obj)
    {
        switch (TypeOf(obj)) {
        case LispObjectType::Nil:
            return "NIL";
        case LispObjectType::Number:
        {
            std::ostringstream s;
            s << NumVal(obj);
            return s.str();
        }
        case LispObjectType::Symbol:
            return StringVal(obj);
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
        default:
            return "";
        }
    }

}
