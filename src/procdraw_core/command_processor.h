#pragma once

#include "procdraw_core/interpreter/lisp_reader.h"
#include <string>

namespace procdraw {

class CommandProcessor
{
public:
  virtual std::string DoCommand(const std::string& cmd) = 0;
  virtual BalancedState CheckCommand(const std::string& cmd) = 0;
  virtual ~CommandProcessor() {}
};
}
