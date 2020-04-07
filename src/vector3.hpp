#ifndef __VECTOR3_HPP__
#define __VECTOR3_HPP__

#include <cstdlib>

struct Vector3 {
  double x, y, z;
  double epsilon;
  Vector3(double a = 0.0, double b = 0.0, double c = 0.0)
      : x(a), y(b), z(c), epsilon(5.0e-7) {}
  bool operator==(const Vector3 &v) const {
    return std::abs(x - v.x) < epsilon && std::abs(y - v.y) < epsilon &&
           std::abs(z - v.z) < epsilon;
  }
  bool operator<(const Vector3 &v) const {
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
};

typedef struct Vector3 V3;

#endif
