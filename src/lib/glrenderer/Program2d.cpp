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

#include <glad/glad.h>
#include "Program2d.h"
#include "GLUtils.h"

namespace Procdraw {

Program2d::Program2d()
{
    static const GLchar* vertexShaderSource[] = {
        "#version 150                                               \n"
        "uniform mat4 projection;                                   \n"
        "in vec2 position;                                          \n"
        "void main(void)                                            \n"
        "{                                                          \n"
        "    gl_Position = projection * vec4(position.xy, 0, 1);    \n"
        "}                                                          \n"};

    static const GLchar* fragmentShaderSource[] = {
        "#version 150                                   \n"
        "uniform vec4 material_color;                   \n"
        "out vec4 color;                                \n"
        "void main(void)                                \n"
        "{                                              \n"
        "    color = material_color;                    \n"
        "}                                              \n"};

    program_ = CompileProgram(vertexShaderSource, fragmentShaderSource,
                              {{0, "position"}});

    projectionLoc_ = glGetUniformLocation(program_, "projection");
    colorLoc_ = glGetUniformLocation(program_, "material_color");
}

Program2d::~Program2d()
{
    glDeleteProgram(program_);
}

} // namespace Procdraw
