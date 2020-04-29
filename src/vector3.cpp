#include "vector3.h"

#include <cmath>
#include <cstdlib>

#define EPSILON 1.0e-7

Vector3::Vector3(double a = 0.0, double b = 0.0, double c = 0.0)
    : x(a), y(b), z(c), epsilon(EPSILON) {}

bool Vector3::operator==(const Vector3 &v) const {
  return std::abs(x - v.x) < epsilon && std::abs(y - v.y) < epsilon &&
         std::abs(z - v.z) < epsilon;
}
bool Vector3::operator<(const Vector3 &v) const {
  if (std::abs(x - v.x) >= epsilon) {
    return x < v.x;
  } else if (std::abs(y - v.y) >= epsilon) {
    return y < v.y;
  } else if (std::abs(z - v.z) >= epsilon) {
    return z < v.z;
  } else {
    return false;
  }
}

double Vector3::mod(const Vector3 &v) {
  return std::sqrt(std::pow(x - v.x, 2) + std::pow(y - v.y, 2) +
                   std::pow(z - v.z, 2));
}
