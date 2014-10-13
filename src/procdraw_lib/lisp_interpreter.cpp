#include "lisp_interpreter.h"

namespace procdraw {

    bool LispInterpreter::Atom(LispObjectPtr obj)
    {
        return mem_.GetType(obj) != LispObjectType::Cell;
    }

    LispObjectPtr LispInterpreter::Read(const std::string &str)
    {
        return reader_.Read(str);
    }

    std::string LispInterpreter::PrintString(LispObjectPtr obj)
    {
        switch (mem_.GetType(obj)) {
        case LispObjectType::Nil:
            return "NIL";
        case LispObjectType::Number:
        {
            std::ostringstream s;
            s << mem_.NumVal(obj);
            return s.str();
        }
        case LispObjectType::Symbol:
            return mem_.StringVal(obj);
        case LispObjectType::Cell:
        {
            std::string s("(");
            LispObjectPtr n = obj;
            while (!mem_.Null(n)) {
                s.append(PrintString(mem_.Car(n)));
                n = mem_.Cdr(n);
                if (!mem_.Null(n) && Atom(n)) {
                    s.append(" . ");
                    s.append(PrintString(n));
                    n = LispMemory::Nil;
                }
                if (!mem_.Null(n)) {
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
