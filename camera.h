#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

namespace procdraw {

    class Camera {
    public:
        Camera();
        glm::mat4 ViewProjectionMatrix();
    private:
        glm::mat4 viewProjectionMatrix_;
    };

}
