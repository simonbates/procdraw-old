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
#include "Cube.h"
#include "GLUtils.h"
#include "Vertex.h"
#include <glm/gtc/type_ptr.hpp>

namespace Procdraw {

Cube::Cube()
{
    auto p1 = glm::vec3(1.0f, 1.0f, 1.0f);
    auto p2 = glm::vec3(1.0f, 1.0f, -1.0f);
    auto p3 = glm::vec3(-1.0f, 1.0f, -1.0f);
    auto p4 = glm::vec3(-1.0f, 1.0f, 1.0f);
    auto p5 = glm::vec3(1.0f, -1.0f, 1.0f);
    auto p6 = glm::vec3(1.0f, -1.0f, -1.0f);
    auto p7 = glm::vec3(-1.0f, -1.0f, -1.0f);
    auto p8 = glm::vec3(-1.0f, -1.0f, 1.0f);

    auto n1 = TriangleNormal(p1, p4, p8);
    auto n2 = TriangleNormal(p1, p5, p6);
    auto n3 = TriangleNormal(p2, p6, p7);
    auto n4 = TriangleNormal(p3, p7, p8);
    auto n5 = TriangleNormal(p1, p2, p3);
    auto n6 = TriangleNormal(p5, p8, p7);

    Vertex vertices[] = {
        Vertex(p1, n1), Vertex(p4, n1), Vertex(p8, n1), Vertex(p5, n1),
        Vertex(p1, n2), Vertex(p5, n2), Vertex(p6, n2), Vertex(p2, n2),
        Vertex(p2, n3), Vertex(p6, n3), Vertex(p7, n3), Vertex(p3, n3),
        Vertex(p3, n4), Vertex(p7, n4), Vertex(p8, n4), Vertex(p4, n4),
        Vertex(p1, n5), Vertex(p2, n5), Vertex(p3, n5), Vertex(p4, n5),
        Vertex(p5, n6), Vertex(p8, n6), Vertex(p7, n6), Vertex(p6, n6)};

    // clang-format off
    GLushort indices[] = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
                          8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
                          16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
    // clang-format on

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Bind vertex buffer
    glGenBuffers(1, &vertexBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, x)));
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, nx)));
    glEnableVertexAttribArray(1);

    // Indices
    glGenBuffers(1, &indexBuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vertexBuffer_);
    glDeleteBuffers(1, &indexBuffer_);
}

void Cube::Draw()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

} // namespace Procdraw
