#pragma once

#include "lisp_interpreter.h"

namespace procdraw {

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Car(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Cdr(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Clear(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Difference(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Eq(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Get(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Keys(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Lerp(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_MakeTable(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_MapRange(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Norm(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Product(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Put(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Quotient(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Sum(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);
    LispObjectPtr lisp_Wrap(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env, void *data);

}