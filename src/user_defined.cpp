#include "user_defined.h"

#include <algorithm>

/**
 * 请实现这些函数，它们是不可缺少的，功能详见user_defined.h
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

std::vector<int> GetTimeStepsVector() {
  return std::vector<int>({0, 34, 117, 144});
}

double GetPrec(double x, double y, double z) {
  if (y < -0.020 || y > 0.020) {
    return 4.0;
  }
  return 1.0;
}
