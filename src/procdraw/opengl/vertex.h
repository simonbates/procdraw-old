#pragma once

#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"
#include <GL/gl3w.h>

namespace procdraw {

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
}
