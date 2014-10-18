#include "lisp_interpreter.h"

namespace procdraw {

    LispInterpreter::LispInterpreter()
    {
        InitNil();
        symbols_ = Nil;
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

    bool LispInterpreter::Atom(LispObjectPtr obj)
    {
        return TypeOf(obj) != LispObjectType::Cons;
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
