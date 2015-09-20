#pragma once

#include <GL/glew.h>

namespace procdraw {

    void ThrowOnGlewError(GLenum err);
    GLuint CompileProgram(const GLchar **vertexShaderSource, const GLchar **fragmentShaderSource);

}
