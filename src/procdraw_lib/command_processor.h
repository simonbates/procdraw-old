#pragma once

#include <string>

namespace procdraw {

    class CommandProcessor {
    public:
        virtual void DoCommand(const std::wstring &cmd) = 0;
        virtual ~CommandProcessor() { }
    };

}
