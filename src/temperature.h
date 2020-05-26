#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#include <string>

/**
 * 温度类型，支持任意精度
 */
class Temp_t {
private:
  long long intTemp; /* 对温度除以精度的商四舍五入取整，下称化整温度 */
  double prec; /* 精度，例如0.1、1、2…… */
  double epsilon; /* 当精度差的绝对值小于这个变量时，认为精度相同 */
public:
  Temp_t();               /* 空白构造 */
  Temp_t(double, double); /* 参数表：温度、精度 */
public:
  long long getIntTemp(); /* 获得化整温度 */
  double getPrec();       /* 获得精度 */
  double getTemp();       /* 获得双精度浮点数型温度 */
  double updateIntTemp(long long); /* 更新化整温度，传入化整温差，返回新温度 */
  long long updateTemp(double); /* 更新温度，传入新温度，返回化整温差 */
  bool operator==(const Temp_t &) const; /* 精度相同且化整温度相同 */
  bool operator!=(const Temp_t &) const; /* 对“==”取反 */
};

typedef Temp_t T_T;

#endif
