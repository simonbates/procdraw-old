#include "stdafx.h"
#include "Printer.h"
#include "Interpreter.h"
#include <sstream>

namespace Procdraw {

    std::string Printer::PrintToString(Interpreter* interpreter)
    {
        switch (interpreter->Type()) {
        case ObjType::Boolean:
            if (interpreter->PopBoolean() == true) {
                return "true";
            }
            else {
                return "false";
            }
        case ObjType::ConsPtr: {
            std::string s("(");
            while (true) {
                interpreter->Next();
                // Car
                s.append(PrintToString(interpreter));
                // Cdr
                if (interpreter->IsNull()) {
                    interpreter->Drop(); // The terminating nil
                    break;
                }
                else {
                    if (interpreter->IsAtom()) {
                        s.append(" . ");
                        s.append(PrintToString(interpreter));
                        break;
                    }
                    else {
                        s.append(" ");
                    }
                }
            }
            s.append(")");
            return s;
        }
        case ObjType::Expr: {
            std::string s("(lambda ");
            interpreter->Dup();
            interpreter->ExprParams();
            s.append(PrintToString(interpreter));
            s.append(" ");
            interpreter->ExprBody();
            s.append(PrintToString(interpreter));
            s.append(")");
            return s;
        }
        case ObjType::Null:
            interpreter->Drop();
            return "nil";
        case ObjType::Real: {
            std::ostringstream s;
            s << interpreter->PopReal();
            return s.str();
        }
        case ObjType::SymbolPtr:
            return interpreter->PopSymbol();
        default:
            return "";
        }
    }

}
