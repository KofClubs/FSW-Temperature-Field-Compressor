#ifndef __USER_DEFINED_H__
#define __USER_DEFINED_H__

#include <string>
#include <vector>

/**
 * 这些函数支持用户实现，它们能提高压缩表现
 *
 * 获得指定时间步的文件名，例如第2个时间步的数据文件名被指定成002.txt
 * 参数表：时间步
 * 返回值：文件名
 */
std::string GetFilename(int);

/**
 * 获得搅拌头轴肩半径
 * 返回值：搅拌头轴肩半径
 */
double GetRadiusOfShoulder();

/**
 * 获得指定时间步的搅拌针头部坐标
 * 参数表：时间步、搅拌针头部X、Y、Z坐标
 */
void GetCurrentWeld(int, double &, double &, double &);

/**
 * 获得时间步向量{e0,e1,e2,e3}
 * [e0, e3)是搅拌摩擦焊全过程时间步区间，[e1,e2)被搅拌头X方向移动时间步区间包含
 * 返回值：时间步向量
 */
std::vector<int> GetTimeStepsVector();

/**
 * 获得指定坐标体元的精度
 * 参数表：X、Y、Z坐标
 * 返回值：精度
 */
double GetPrec(double, double, double);

#endif
