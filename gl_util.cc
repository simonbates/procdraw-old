#include "gl_util.h"
#include <stdexcept>

namespace procdraw {

    void ThrowOnGlewError(GLenum err)
    {
        if (err != GLEW_OK) {
            throw std::runtime_error(reinterpret_cast<const char*>(glewGetErrorString(err)));
        }
    }

    GLuint CompileProgram(const GLchar **vertexShaderSource, const GLchar **fragmentShaderSource)
    {
        // Vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

}
