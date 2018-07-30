#pragma once

#include <string>

namespace Procdraw {

    class Interpreter;

    class Printer
    {
    public:
        std::string PrintToString(Interpreter* interpreter);
    };

}
