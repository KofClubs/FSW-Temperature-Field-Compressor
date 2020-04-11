#ifndef __VECTOR3_H__
#define __VECTOR3_H__

struct Vector3 {
  double x, y, z;
  double epsilon;
  Vector3(double, double, double);
  bool operator==(const Vector3 &v) const;
  bool operator<(const Vector3 &v) const;
};

typedef Vector3 V3;

#endif
