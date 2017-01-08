#pragma once

#include <GL/gl3w.h>
#include <map>

namespace procdraw {

GLuint CompileProgram(const GLchar** vertexShaderSource,
    const GLchar** fragmentShaderSource,
    std::map<GLuint, const GLchar*> attribLocations);
GLuint CompileShader(const GLchar** shaderSource, GLenum shaderType);
}
