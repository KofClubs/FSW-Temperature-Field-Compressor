#ifndef __USER_DEFINED_HPP__
#define __USER_DEFINED_HPP__

#include <algorithm>
#include <string>

/**
 * 这些函数支持用户实现，它们能提高压缩表现
 *
 * 获得指定时间步的文件名
 * 参数表：时间步
 * 返回值：文件名
 */
std::string GetFilename(int t) {
  char a, b, c;
  a = (char)(t / 100);
  b = (char)((t - 100 * a) / 10);
  c = (char)(t - 100 * a - 10 * b);
  std::string s({a + 48, b + 48, c + 48});
  s.insert(3, ".txt");
  return s;
}

/**
 * 获得指定时间步的搅拌针头部坐标
 * 参数表：时间步、搅拌针头部X、Y、Z坐标
 */
void GetCurrentWeld(int t, double &x, double &y, double &z) {
  if (t < 117) {
    x = std::max(-0.125, -0.125 + 0.003 * (t - 33));
    z = std::max(0.004 - 0.00046 * t, -0.0043);
  } else {
    x = 0.125;
    z = 0.005;
  }
  y = 0.0;
}

/**
 * 获得准稳态时间步区间[lo, hi)
 * 参数表：准稳态开始、被选成零状态的时间步、结束时间步
 */
void GetQuasiSteadyStateInterval(int &lo, int &mi, int &hi) {
  lo = 57;
  mi = 97;
  hi = 117;
}

#endif
