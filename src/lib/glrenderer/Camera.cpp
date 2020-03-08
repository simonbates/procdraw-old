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

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Procdraw {

Camera::Camera()
{
    auto eye = glm::vec3(0.0f, 0.0f, 10.0f);
    auto center = glm::vec3(0.0f, 0.0f, 0.0f);
    auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    auto viewMatrix = glm::lookAt(eye, center, up);
    auto projectionMatrix = glm::perspective(0.785f, 1.0f, 1.0f, 100.0f);
    viewProjectionMatrix_ = projectionMatrix * viewMatrix;
}

glm::mat4 Camera::ViewProjectionMatrix()
{
    return viewProjectionMatrix_;
}

} // namespace Procdraw
