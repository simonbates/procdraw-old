#include "procdraw/graphics/camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace procdraw {

Camera::Camera()
{
    auto eye = glm::vec3(0.0f, 0.0f, 10.0f);
    auto center = glm::vec3(0.0f, 0.0f, 0.0f);
    auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    auto viewMatrix = glm::lookAt(eye, center, up);
    auto projectionMatrix = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
    viewProjectionMatrix_ = projectionMatrix * viewMatrix;
}

glm::mat4 Camera::ViewProjectionMatrix()
{
    return viewProjectionMatrix_;
}
}
