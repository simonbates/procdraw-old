#include "lisp_interpreter.h"
#include <iostream>
#include <string>

int main()
{
    procdraw::LispInterpreter L;
    std::string exp;
    std::string val;

    while (true) {
        std::cout << "? ";
        std::getline(std::cin, exp);

        // TODO check if not all whitespace, rather than not empty
        if (!exp.empty()) {
            try {
                val = L.PrintString(L.Eval(L.Read(exp)));
            }
            catch (std::exception e) {
                val = e.what();
            }
            std::cout << val << std::endl;
        }
    }
}
