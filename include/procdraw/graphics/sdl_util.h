#pragma once

#include PROCDRAW_SDL2_H

namespace procdraw {

    void ThrowSdlError();

    class SDL {
    public:
        SDL(Uint32 flags);
        ~SDL();
    };

}
