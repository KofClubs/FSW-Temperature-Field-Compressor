#ifndef __COMPRESS_H__
#define __COMPRESS_H__

/**
 * 读入第1帧数据，初始化体积元的坐标和初始温度
 * 参数表：第1帧数据文件路径，输出文件路径
 */
void InitTempField(const char*, const char*);

/**
 * 读入其他数据帧，更新温度场
 * 参数表：当前数据文件路径，输出文件路径
 */
void UpdateTempField(const char*, const char*);

#endif