#include "lisp_interpreter.h"

namespace procdraw {

    LispInterpreter::LispInterpreter()
    {
        InitNil();
        symbols_ = Nil;
        S_ADD = SymbolRef("ADD");
        S_QUOTE = SymbolRef("QUOTE");
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
                return (Car(Cdr(exp)));
            }
            else if (Eq(Car(exp), S_ADD)) {
                return Add(Evlis(Cdr(exp), env));
            }
        }

        return Nil;
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
