#pragma once

#include "command_processor.h"

namespace procdraw {

    class CLI {
    public:
        CLI(CommandProcessor *p);
        ~CLI();
        void Poll();
    };

}
