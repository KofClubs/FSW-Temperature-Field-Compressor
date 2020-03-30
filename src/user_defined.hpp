#ifndef __USER_DEFINED_HPP__
#define __USER_DEFINED_HPP__

#include <algorithm>
#include <string>
#include <vector>

/**
 * 这些函数支持用户实现，它们能提高压缩表现
 *
 * 获得存储坐标的文件名，请任意指定
 * 返回值：文件名
 */
std::string GetCoorFilename() {
  std::string s("coor.txt");
  return s;
}

/**
 * 获得指定时间步的文件名，例如第2个时间步的数据文件名被指定成002.txt
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
 * 获得准稳态时间步区间[0, lo, mi1, mi2, ... ,mi(n-1), hi, N)
 * 向量的元素单调递增，至少包含“0”和“N”
 * 向量顺序前2个元素分别是0和准稳态开始时间步，逆序前2个元素分别是时间步数目和准稳态结束时间步
 * 返回值：时间步向量
 */
std::vector<int> GetTimeStepsVector() {
  std::vector<int> v({0, 57, 77, 97, 117, 144});
  return v;
}

#endif
