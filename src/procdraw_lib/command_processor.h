#pragma once

#include <string>

namespace procdraw {

    class CommandProcessor {
    public:
        virtual void DoCommand(const std::string &cmd) = 0;
        virtual ~CommandProcessor() { }
    };

}
