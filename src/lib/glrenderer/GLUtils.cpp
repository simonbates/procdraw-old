// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "GLUtils.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

namespace Procdraw {

GLuint CompileProgram(const GLchar** vertexShaderSource,
                      const GLchar** fragmentShaderSource,
                      std::map<GLuint, const GLchar*> attribLocations)
{
    GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader =
        CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    for (const auto& attrib : attribLocations) {
        glBindAttribLocation(program, attrib.first, attrib.second);
    }

    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

GLuint CompileShader(const GLchar** shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, shaderSource, NULL);
    glCompileShader(shader);
    return shader;
}

glm::vec3 TriangleNormal(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
    return glm::normalize(glm::cross(b - a, c - a));
}

} // namespace Procdraw
