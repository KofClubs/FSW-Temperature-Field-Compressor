#ifndef __VECTOR3_H__
#define __VECTOR3_H__

struct Vector3 {
  double x, y, z;
  double epsilon;
  Vector3(double, double, double);
  bool operator==(const Vector3 &) const;
  bool operator<(const Vector3 &) const;
  double mod(const Vector3 &);
};

typedef Vector3 V3;

#endif
