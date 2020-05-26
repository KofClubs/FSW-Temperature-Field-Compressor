#ifndef __VECTOR3_H__
#define __VECTOR3_H__

/**
 * 三维空间向量
 */
struct Vector3 {
  double x, y, z; /* X、Y、Z坐标 */
  double epsilon; /* 当任意维坐标差的绝对值均小于这个变量时，认为相等 */
  Vector3(double, double, double); /* 赋值构造，缺省值{0.0, 0.0, 0.0} */
  bool operator==(const Vector3 &) const; /* 二点坐标认为相等 */
  bool operator<(const Vector3 &) const;  /* 用来排序，无实际意义 */
  double mod(const Vector3 &);            /* 模 */
};

typedef Vector3 V3;

#endif
