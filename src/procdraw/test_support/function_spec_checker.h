#pragma once

#include "procdraw/interpreter/lisp_interpreter.h"
#include <string>
#include <tinyxml2.h>
#include <vector>

namespace procdraw_test {

class FunctionSpecChecker {
public:
    bool Check(const char* filename, int expectedNumTests);
    std::vector<std::string> GetMessages();

private:
    std::vector<std::string> messages_;
    bool CheckExample(tinyxml2::XMLElement* example,
                      procdraw::LispInterpreter* L,
                      int* numTests);
};
}
