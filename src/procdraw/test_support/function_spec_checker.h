#pragma once

#include "procdraw/interpreter/lisp_interpreter.h"
#include "tinyxml2.h"  // NOLINT
#include <string>
#include <vector>

namespace procdraw_test {

class FunctionSpecChecker
{
public:
  bool Check(const char* filename, int expectedNumTests);
  std::vector<std::string> GetMessages();

private:
  std::vector<std::string> messages_;
  bool CheckExample(tinyxml2::XMLElement* example, procdraw::LispInterpreter* L,
                    int* numTests);
};
}
