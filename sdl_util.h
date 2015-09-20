#pragma once

#include <SDL2/SDL.h>

namespace procdraw {

    void ThrowSdlError();

    class SDL {
    public:
        SDL(Uint32 flags);
        ~SDL();
    };

}
