#include "compress.h"

#include <cstdlib>
#include <fstream>
#include <string>

#include "exit_status.h"

const unsigned int NumOfNoneDataLine = 18;
const std::string CoorFilename = "/coor.dat";	/* 体积元坐标 */
const std::string TempFilename = "/temp.dat";	/* 温度场及变化 */
const std::string CurrFilename = "/curr.dat";	/* 当前温度场 */

void InitTempField(const char *inputFile, const char *outputDir)
{
	std::string foutCoorFile = CoorFilename, foutTempFile = TempFilename, foutCurrFile = CurrFilename;
	foutCoorFile.insert(0, outputDir);
	foutTempFile.insert(0, outputDir);
	foutCurrFile.insert(0, outputDir);
	std::ifstream fin(inputFile);
	std::ofstream foutCoor(foutCoorFile);
	std::ofstream foutTemp(foutTempFile);
	std::ofstream foutCurr(foutCurrFile);
	if (!fin.is_open())
	{
		exit(DATA_FILE_NOT_FOUND);
	}
	std::string buffer;
	for (unsigned int i = 0; i < NumOfNoneDataLine; i++)
	{
		if (fin.eof())
		{
			exit(DATA_FILE_EMPTY);
		}
		getline(fin, buffer);
	}
	std::string x, y, z;
	double t;
	if (fin.eof())
	{
		exit(DATA_FILE_EMPTY);
	}
	fin >> x >> y >> z >> t;
	foutCoor << x << "\t" << y << "\t" << z << std::endl;
	foutTemp << (short)(t + 0.5) << std::endl;
	foutCurr << (short)(t + 0.5) << std::endl;
	if (fin.eof())
	{
		return;
	}
	std::string predX, predY, predZ;	/* 即时记录上一个坐标 */
	// 分别读取X坐标和(Y, Z)坐标，原因是数据文件末尾是换行，换行长度跟操作系统相关
	predX = x;
	fin >> x;
	while (!fin.eof())
	{
		predY = y;
		predZ = z;
		fin >> y >> z >> t;
		foutCoor << (x == predX ? "*" : x) << "\t" << (y == predY ? "*" : y) << "\t" << (z == predZ ? "*" : z) << std::endl;
		foutTemp << (short)(t + 0.5) << std::endl;
		foutCurr << (short)(t + 0.5) << std::endl;
		predX = x;
		fin >> x;
	}
	fin.close();
	foutCoor.close();
	foutTemp.close();
	foutCurr.close();
}

void UpdateTempField(const char *inputFile, const char *outputDir)
{
	// Do sth.
	// 函数返回前，把当前温度场输出到文件。
	// 在InitTempField添加功能，拷贝一份当前温度场。
	// 检查outputDir是否存在初始化文件。
}