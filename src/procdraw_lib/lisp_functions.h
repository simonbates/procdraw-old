#pragma once

#include "lisp_interpreter.h"

namespace procdraw {

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

    LispObjectPtr lisp_Difference(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

    LispObjectPtr lisp_Product(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

    LispObjectPtr lisp_Quotient(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

    LispObjectPtr lisp_Sum(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

}
