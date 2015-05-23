#pragma once

#include "procdraw_app_sdl.h"
#include "lisp_interpreter.h"

namespace procdraw {

    void RegisterProcDrawAppSdlFunctionsForLisp(ProcDrawAppSdl *a, LispInterpreter *L);

}
