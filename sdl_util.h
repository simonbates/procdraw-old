#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace procdraw {

    void ThrowSdlError();
    void ThrowOnGlewError(GLenum err);

    class SDL {
    public:
        SDL(Uint32 flags);
        ~SDL();
    };

}
