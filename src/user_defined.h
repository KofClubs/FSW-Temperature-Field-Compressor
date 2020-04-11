#ifndef __USER_DEFINED_H__
#define __USER_DEFINED_H__

#include <string>
#include <vector>

/**
 * 这些函数支持用户实现，它们能提高压缩表现
 *
 * 获得存储坐标的文件名，请任意指定
 * 返回值：文件名
 */
std::string GetCoorFilename();

/**
 * 获得指定时间步的文件名，例如第2个时间步的数据文件名被指定成002.txt
 * 参数表：时间步
 * 返回值：文件名
 */
std::string GetFilename(int);

/**
 * 获得指定时间步的搅拌针头部坐标
 * 参数表：时间步、搅拌针头部X、Y、Z坐标
 */
void GetCurrentWeld(int, double &, double &, double &);

/**
 * 获得准稳态时间步区间[0, lo, mi1, mi2, ... ,mi(n-1), hi, N)
 * 向量的元素单调递增，至少包含“0”和“N”
 * 向量顺序前2个元素分别是0和准稳态开始时间步，逆序前2个元素分别是时间步数目和准稳态结束时间步
 * 返回值：时间步向量
 */
std::vector<int> GetTimeStepsVector();

#endif
