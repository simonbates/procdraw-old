#pragma once

#include "lisp_interpreter.h"

namespace procdraw {

    LispObjectPtr lisp_Add(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

    LispObjectPtr lisp_Apply(LispInterpreter *L, LispObjectPtr args, LispObjectPtr env);

}
