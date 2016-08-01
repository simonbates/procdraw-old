#pragma once

#include <GL/glew.h>
#include <map>

namespace procdraw {

    void ThrowOnGlewError(GLenum err);
    GLuint CompileProgram(const GLchar **vertexShaderSource, const GLchar **fragmentShaderSource,
                          std::map<GLuint, const GLchar*> attribLocations);
    GLuint CompileShader(const GLchar **shaderSource, GLenum shaderType);

}
