#pragma once

#include "lisp_interpreter.h"

namespace procdraw {

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Car(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Cdr(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Clear(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Difference(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Eq(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Get(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_MakeTable(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_MapRange(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Product(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Put(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Quotient(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_Sum(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);
    LispObjectPtr lisp_WrapRange(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

}
