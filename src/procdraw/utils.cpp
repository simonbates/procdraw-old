#include "procdraw/utils.h"
#include <cmath>

namespace procdraw {

bool
ApproximatelyEqual(double a, double b, double epsilon)
{
  return fabs(a - b) < epsilon;
}

double
Lerp(double start, double stop, double val)
{
  return (1 - val) * start + val * stop;
}

double
MapRange(double start1, double stop1, double start2, double stop2, double val)
{
  return start2 + ((val - start1) * (stop2 - start2)) / (stop1 - start1);
}

double
Norm(double start, double stop, double val)
{
  return (val - start) / (stop - start);
}

int
PowerOf2Gte(int n)
{
  if (n <= 0) {
    return 0;
  }
  int powerOf2 = 1;
  while (powerOf2 < n) {
    powerOf2 *= 2;
  }
  return powerOf2;
}

double
Wrap(double start, double stop, double val)
{
  return val - floor((val - start) / (stop - start)) * (stop - start);
}

glm::vec3
TriangleNormal(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
  return glm::normalize(glm::cross(b - a, c - a));
}
}
