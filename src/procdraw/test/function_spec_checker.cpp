#include "procdraw/test/function_spec_checker.h"
#include <string>
#include <vector>

namespace procdraw_test {

bool
FunctionSpecChecker::Check(const char* filename, int expectedNumTests)
{
  messages_.clear();
  int numTests = 0;
  procdraw::LispInterpreter L;
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError loadError = doc.LoadFile(filename);

  if (loadError != tinyxml2::XMLError::XML_NO_ERROR) {
    messages_.push_back(std::string("Error loading file: ") + filename);
    return false;
  }

  bool allPassed = true;

  tinyxml2::XMLElement* functionSpec = doc.FirstChildElement("function-spec");
  if (functionSpec != NULL) {
    tinyxml2::XMLElement* examples =
      functionSpec->FirstChildElement("examples");
    if (examples != NULL) {
      tinyxml2::XMLElement* example = examples->FirstChildElement("example");
      while (example != NULL) {
        if (!CheckExample(example, &L, &numTests)) {
          allPassed = false;
        }
        example = example->NextSiblingElement("example");
      }
    }
  }

  if (!allPassed) {
    return false;
  } else {
    if (expectedNumTests != numTests) {
      messages_.push_back("EXPECTED " + std::to_string(expectedNumTests) +
                          " TESTS BUT " + std::to_string(numTests) +
                          " WERE RUN");
      return false;
    } else {
      return true;
    }
  }
}

std::vector<std::string>
FunctionSpecChecker::GetMessages()
{
  return messages_;
}

bool
FunctionSpecChecker::CheckExample(tinyxml2::XMLElement* example,
                                  procdraw::LispInterpreter* L, int* numTests)
{
  bool testPassed = false;

  const char* expr = example->Attribute("expr");
  if (expr != NULL) {
    const char* expectedValue = example->Attribute("value");
    if (expectedValue != NULL) {
      std::string result = L->PrintToString(L->Eval(L->Read(expr)));
      *numTests = *numTests + 1;
      if (result == expectedValue) {
        testPassed = true;
      } else {
        testPassed = false;
        messages_.push_back(std::string("EXPR: ") + expr + " EXPECTED: " +
                            expectedValue + " ACTUAL: " + result);
      }
    }
  }

  return testPassed;
}
}
