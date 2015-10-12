#include "lisp_printer.h"
#include "lisp_interpreter.h"

namespace procdraw {

    std::string LispPrinter::PrintToString(LispInterpreter *L, LispObjectPtr obj)
    {
        switch (L->TypeOf(obj)) {
        case LispObjectType::Null:
            return "nil";
        case LispObjectType::Number: {
            std::ostringstream s;
            s << L->NumVal(obj);
            return s.str();
        }
        case LispObjectType::Symbol:
            return L->SymbolName(obj);
        case LispObjectType::Cons: {
            if (L->Eq(L->Car(obj), L->SymbolRef("quote"))) {
                return "'" + PrintToString(L, L->Cadr(obj));
            }
            else {
                std::string s("(");
                LispObjectPtr n = obj;
                while (!L->Null(n)) {
                    s.append(PrintToString(L, L->Car(n)));
                    n = L->Cdr(n);
                    if (!L->Null(n) && L->Atom(n)) {
                        s.append(" . ");
                        s.append(PrintToString(L, n));
                        n = L->Nil;
                    }
                    if (!L->Null(n)) {
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
            return L->BoolVal(obj) ? "true" : "false";
        case LispObjectType::String:
            return "\"" + L->StringVal(obj) + "\"";
        case LispObjectType::Eof:
            return "<Eof>";
        default:
            return "";
        }
    }

}
