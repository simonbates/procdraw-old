#pragma once

#include <GL/glew.h>

namespace procdraw {

    void ThrowSdlError();
    void ThrowOnGlewError(GLenum err);

}
