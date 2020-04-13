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

#ifndef PROCDRAW_GLRENDERER_H
#define PROCDRAW_GLRENDERER_H

#include "Camera.h"
#include "Cube.h"
#include "FlatProgram.h"
#include "Program2d.h"
#include "Rectangle.h"
#include <glm/glm.hpp>

namespace Procdraw {

class GLRenderer {
public:
    GLRenderer(int width, int height);
    void Background(float h, float s, float v);
    void Begin2D();
    void Begin3D();
    void Colour(float h, float s, float v, float a = 1.0f);
    void DrawCube();
    void DrawRectangle(int x, int y, int w, int h);
    void RotateX(float turns);
    void RotateY(float turns);
    void RotateZ(float turns);

private:
    FlatProgram program3d_;
    Program2d program2d_;
    Camera camera_;
    Cube cube_;
    Rectangle rectangle_;
    glm::mat4 worldMatrix_;
    glm::vec4 lightDirection_;
    glm::vec4 lightColor_;
    glm::vec4 ambientLightColor_;
    float materialR_;
    float materialG_;
    float materialB_;
    float materialA_;
    void ResetMatrix();
    void UpdateUniformsForObject();
};

} // namespace Procdraw

#endif
