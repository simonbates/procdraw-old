#pragma once

#include <SDL2/SDL.h>

namespace procdraw {

    class ReplThread {
    public:
        ReplThread(const char *name);
        ~ReplThread();
    private:
        SDL_Thread *thread_;
    };

}
