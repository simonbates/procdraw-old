#pragma once

#include <SDL2/SDL.h>

namespace procdraw {

void ThrowSdlError();

class SDL {
public:
    explicit SDL(Uint32 flags);
    ~SDL();
};
}
