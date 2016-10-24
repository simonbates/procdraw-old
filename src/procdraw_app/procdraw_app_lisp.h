#pragma once

#include "procdraw_app/procdraw_app.h"
#include "procdraw_core/interpreter/lisp_interpreter.h"

namespace procdraw {

void RegisterProcdrawAppFunctions(ProcdrawApp* app, LispInterpreter* L);
}
