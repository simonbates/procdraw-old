#include "procdraw/graphics/sdl_util.h"
#include <stdexcept>

namespace procdraw {

void
ThrowSdlError()
{
  throw std::runtime_error(SDL_GetError());
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
