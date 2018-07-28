#pragma once

#include <string>

namespace procdraw {

    class Interpreter;

    class Printer
    {
    public:
        std::string PrintToString(Interpreter* interpreter);
    };

}
