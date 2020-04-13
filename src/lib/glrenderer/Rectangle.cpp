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
#include "Rectangle.h"

namespace Procdraw {

Rectangle::Rectangle()
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Bind vertex buffer
    glGenBuffers(1, &vertexBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_DYNAMIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

Rectangle::~Rectangle()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertexBuffer_);
}

void Rectangle::Draw(int x, int y, int w, int h)
{
    vertices_[0] = x;
    vertices_[1] = y;
    vertices_[2] = x;
    vertices_[3] = y + h;
    vertices_[4] = x + w;
    vertices_[5] = y;
    vertices_[6] = x + w;
    vertices_[7] = y + h;

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_), vertices_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

} // namespace Procdraw
