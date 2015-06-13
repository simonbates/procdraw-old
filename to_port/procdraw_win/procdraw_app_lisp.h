#pragma once

#include "procdraw_app.h"
#include "lisp_interpreter.h"

namespace procdraw {

    void RegisterProcDrawAppFunctionsForLisp(ProcDrawApp *a, LispInterpreter *L);

}
