#pragma once

#include "procdraw_app.h"
#include <SDL2/SDL.h>

namespace procdraw {

    class ReplThread {
    public:
        ReplThread(const char *name, ProcdrawApp *app);
        ~ReplThread();
    private:
        SDL_Thread *thread_;
    };

}
