#include "sdl_util.h"
#include <stdexcept>
#include <SDL2/SDL.h>

namespace procdraw {

    void ThrowSdlError()
    {
        throw std::runtime_error(SDL_GetError());
    }

    void ThrowOnGlewError(GLenum err)
    {
        if (err != GLEW_OK) {
            throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(err)));
        }
    }

}
