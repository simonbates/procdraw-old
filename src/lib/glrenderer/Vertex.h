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

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Procdraw {

struct Vertex {
    // Position
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
    // Normal
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    Vertex(glm::vec3 pos, glm::vec3 normal)
    {
        this->x = pos.x;
        this->y = pos.y;
        this->z = pos.z;
        this->w = 1.0f;
        this->nx = normal.x;
        this->ny = normal.y;
        this->nz = normal.z;
    }
};

} // namespace Procdraw
