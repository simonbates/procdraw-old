#include "sdl_util.h"
#include <stdexcept>

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

    SDL::SDL(Uint32 flags)
    {
        if (SDL_Init(flags) != 0) {
            ThrowSdlError();
        }
    }

    SDL::~SDL()
    {
        SDL_Quit();
    }

}
