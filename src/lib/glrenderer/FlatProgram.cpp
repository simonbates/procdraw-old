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
#include "FlatProgram.h"
#include "GLUtils.h"

namespace Procdraw {

FlatProgram::FlatProgram()
{
    static const GLchar* vertexShaderSource[] = {
        "#version 150                                                   \n"
        "uniform mat4 world_view_projection;                            \n"
        "uniform vec4 light_direction;                                  \n"
        "uniform vec4 light_color;                                      \n"
        "uniform vec4 ambient_light_color;                              \n"
        "uniform vec4 material_color;                                   \n"
        "in vec4 position;                                              \n"
        "in vec3 normal;                                                \n"
        "out vec4 vs_color;                                             \n"
        "void main(void)                                                \n"
        "{                                                              \n"
        "    gl_Position = world_view_projection * position;            \n"
        "    float diff_factor = max(dot(normal, light_direction.xyz), 0);      \n"
        "    vec4 diff_component = (light_color * material_color * diff_factor);\n"
        "    vec4 ambient_component = (ambient_light_color * material_color);   \n"
        "    vs_color.rgb = diff_component.rgb + ambient_component.rgb; \n"
        "    vs_color.a = material_color.a;                             \n"
        "}                                                              \n"};

    static const GLchar* fragmentShaderSource[] = {
        "#version 150                                   \n"
        "in vec4 vs_color;                              \n"
        "out vec4 color;                                \n"
        "void main(void)                                \n"
        "{                                              \n"
        "    color = vs_color;                          \n"
        "}                                              \n"};

    program_ = CompileProgram(vertexShaderSource, fragmentShaderSource,
                              {{0, "position"}, {1, "normal"}});

    worldViewProjectionLoc_ =
        glGetUniformLocation(program_, "world_view_projection");
    lightDirectionLoc_ = glGetUniformLocation(program_, "light_direction");
    lightColorLoc_ = glGetUniformLocation(program_, "light_color");
    ambientLightColorLoc_ =
        glGetUniformLocation(program_, "ambient_light_color");
    materialColorLoc_ = glGetUniformLocation(program_, "material_color");
}

FlatProgram::~FlatProgram()
{
    glDeleteProgram(program_);
}

} // namespace Procdraw
