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

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include "GLRenderer.h"
#include "../../lib/Colour.h"
#include "Vertex.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>

// TODO: Rather than draw immediately, record draw commands and execute them
//       in batch. To minimise switching resources. For example, draw all text
//       together.

namespace Procdraw {

GLRenderer::GLRenderer()
    : worldMatrix_(glm::mat4(1.0f)),
      lightDirection_(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)),
      lightColor_(glm::vec4(0.5f, 0.5f, 0.5f, 0.0f)),
      ambientLightColor_(glm::vec4(0.5f, 0.5f, 0.5f, 0.0f)),
      materialR_(0.8f),
      materialG_(0.8f),
      materialB_(0.8f),
      materialA_(1.0f)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program_.Program());
    glEnable(GL_DEPTH_TEST);
}

void GLRenderer::Background(float h, float s, float v)
{
    float r, g, b;
    std::tie(r, g, b) = Hsv2rgb(h, s, v);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ResetMatrix();
}

void GLRenderer::Colour(float h, float s, float v, float a)
{
    std::tie(materialR_, materialG_, materialB_) = Hsv2rgb(h, s, v);
    materialA_ = a;
}

void GLRenderer::DrawCube()
{
    UpdateUniformsForObject();
    glBindVertexArray(cube_.Vao());
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void GLRenderer::RotateX(float turns)
{
    worldMatrix_ =
        glm::rotate(worldMatrix_, turns * 2 * static_cast<float>(M_PI),
                    glm::vec3(1.0f, 0.0f, 0.0f));
}

void GLRenderer::RotateY(float turns)
{
    worldMatrix_ =
        glm::rotate(worldMatrix_, turns * 2 * static_cast<float>(M_PI),
                    glm::vec3(0.0f, 1.0f, 0.0f));
}

void GLRenderer::RotateZ(float turns)
{
    worldMatrix_ =
        glm::rotate(worldMatrix_, turns * 2 * static_cast<float>(M_PI),
                    glm::vec3(0.0f, 0.0f, 1.0f));
}

void GLRenderer::ResetMatrix()
{
    worldMatrix_ = glm::mat4(1.0f);
}

void GLRenderer::UpdateUniformsForObject()
{
    auto worldViewProjection = camera_.ViewProjectionMatrix() * worldMatrix_;
    auto inverseWorldMatrix = glm::inverse(worldMatrix_);
    auto modelSpaceLightDirection =
        glm::normalize(inverseWorldMatrix * lightDirection_);

    glUniformMatrix4fv(program_.WorldViewProjectionLoc(), 1, GL_FALSE,
                       glm::value_ptr(worldViewProjection));
    glUniform4fv(program_.LightDirectionLoc(), 1,
                 glm::value_ptr(modelSpaceLightDirection));
    glUniform4fv(program_.LightColorLoc(), 1, glm::value_ptr(lightColor_));
    glUniform4fv(program_.AmbientLightColorLoc(), 1, glm::value_ptr(ambientLightColor_));
    glUniform4f(program_.MaterialColorLoc(), materialR_, materialG_, materialB_,
                materialA_);
}

} // namespace Procdraw
