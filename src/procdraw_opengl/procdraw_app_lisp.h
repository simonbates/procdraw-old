#pragma once

#include "procdraw_core/interpreter/lisp_interpreter.h"
#include "procdraw_opengl/procdraw_app.h"

namespace procdraw {

void RegisterProcdrawAppFunctions(ProcdrawApp* app, LispInterpreter* L);
}
