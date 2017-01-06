#pragma once

#include "procdraw/core/interpreter/lisp_interpreter.h"
#include "procdraw/opengl/procdraw_app.h"

namespace procdraw {

void RegisterProcdrawAppFunctions(ProcdrawApp* app, LispInterpreter* L);
}
