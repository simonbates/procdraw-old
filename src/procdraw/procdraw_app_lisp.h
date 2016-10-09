#pragma once

#include "procdraw/interpreter/lisp_interpreter.h"
#include "procdraw/procdraw_app.h"

namespace procdraw {

void RegisterProcdrawAppFunctions(ProcdrawApp* app, LispInterpreter* L);
}
