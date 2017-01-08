#pragma once

#include "glm/glm.hpp"

namespace procdraw {

template <typename T> T Clamp(T value, T lower, T upper)
{
    if (value < lower) {
        return lower;
    } else if (value > upper) {
        return upper;
    }
    return value;
}

bool ApproximatelyEqual(double a, double b, double epsilon);
double Lerp(double start, double stop, double val);
double MapRange(
    double start1, double stop1, double start2, double stop2, double val);
double Norm(double start, double stop, double val);
int PowerOf2Gte(int n);
double Wrap(double start, double stop, double val);
glm::vec3 TriangleNormal(
    const glm::vec3 a, const glm::vec3 b, const glm::vec3 c);
}
