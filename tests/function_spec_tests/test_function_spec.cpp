#include "function_spec_checker.h"
#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        return 1;
    }

    procdraw_test::FunctionSpecChecker checker;
    bool passed = checker.Check(argv[1], atoi(argv[2]));

    if (!passed) {
        for (auto message : checker.GetMessages()) {
            std::cerr << message << std::endl;
        }
        return 1;
    }

    return 0;
}
