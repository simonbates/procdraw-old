#pragma once

#include "lisp_memory.h"
#include <string>

namespace procdraw {

class LispInterpreter;

class LispPrinter
{
public:
  std::string PrintToString(LispInterpreter* L, LispObjectPtr obj);
};
}
