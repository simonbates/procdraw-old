#include "lisp_interpreter.h"
#include "tinyxml2.h"
#include <iostream>
#include <stdlib.h>

static bool TestExample(tinyxml2::XMLElement *example, procdraw::LispInterpreter *L, int *numTests)
{
    bool testPassed = false;

    const char *expr = example->Attribute("expr");
    if (expr != NULL) {
        const char *expectedValue = example->Attribute("value");
        if (expectedValue != NULL) {
            std::string result = L->PrintToString(L->Eval(L->Read(expr)));
            *numTests = *numTests + 1;
            if (result == expectedValue) {
                testPassed = true;
            }
            else {
                testPassed = false;
                std::cerr << "EXPR: " << expr;
                std::cerr << " EXPECTED: " << expectedValue;
                std::cerr << " ACTUAL: " << result << std::endl;
            }
        }
    }

    return testPassed;
}

static bool TestFunction(const char *filename, int *numTests)
{
    procdraw::LispInterpreter L;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError loadError = doc.LoadFile(filename);

    if (loadError != tinyxml2::XMLError::XML_NO_ERROR) {
        std::cerr << "Error loading file: " << filename << std::endl;
        return false;
    }

    bool allPassed = true;

    tinyxml2::XMLElement *functionSpec = doc.FirstChildElement("function-spec");
    if (functionSpec != NULL) {
        tinyxml2::XMLElement *examples = functionSpec->FirstChildElement("examples");
        if (examples != NULL) {
            tinyxml2::XMLElement *example = examples->FirstChildElement("example");
            while (example != NULL) {
                if (!TestExample(example, &L, numTests)) {
                    allPassed = false;
                }
                example = example->NextSiblingElement("example");
            }
        }
    }

    return allPassed;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        return 1;
    }

    int numTests = 0;
    bool passed = TestFunction(argv[1], &numTests);

    if (atoi(argv[2]) != numTests) {
        std::cerr << "EXPECTED " << argv[2] << " TESTS BUT " << numTests << " WERE RUN" << std::endl;
        return 1;
    }

    return (passed ? 0 : 1);
}
