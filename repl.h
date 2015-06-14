#pragma once

#include "procdraw_app_sdl.h"
#include <SDL2/SDL.h>

namespace procdraw {

    class ReplThread {
    public:
        ReplThread(const char *name, ProcDrawAppSdl *app);
        ~ReplThread();
    private:
        SDL_Thread *thread_;
    };

}
