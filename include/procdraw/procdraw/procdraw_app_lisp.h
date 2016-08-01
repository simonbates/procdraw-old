#pragma once

#include "procdraw/procdraw/procdraw_app.h"
#include "procdraw/interpreter/lisp_interpreter.h"

namespace procdraw {

    void RegisterProcdrawAppFunctions(ProcdrawApp *app, LispInterpreter *L);

}
