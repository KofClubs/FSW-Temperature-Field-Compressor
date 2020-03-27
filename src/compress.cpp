#include "compress.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "exit_status.h"
#include "user_defined.hpp"

const std::string CoorFilename = "/coor.txt"; /* 体积元坐标 */
const std::string CurrFilename = "/curr.txt"; /* 当前温度场 */

/**
 * 读入第1帧数据，初始化体积元的坐标和初始温度
 * 参数表：第1帧数据文件路径，输出文件路径
 */
void InitTempField(std::string, const char *);

/**
 * 读入其他数据帧，更新温度场
 * 参数表：当前数据文件路径，输出文件路径，当前时间步
 */
void UpdateTempField(std::string, const char *, int);

void CompressTempField(const char *inputDir, int n, const char *outputDir) {
  for (int i = 0; i < n; i++) {
    std::string inputFile = GetFilename(i).insert(0, "/");
    inputFile.insert(0, inputDir);
    if (i == 0) {
      InitTempField(inputFile, outputDir);
    } else {
      UpdateTempField(inputFile, outputDir, i);
    }
  }
}

void InitTempField(std::string inputFile, const char *outputDir) {
  std::string foutCoorFile = CoorFilename, foutTempFile = "/0",
              foutCurrFile = CurrFilename;
  foutCoorFile.insert(0, outputDir);
  foutTempFile.insert(0, outputDir);
  foutCurrFile.insert(0, outputDir);
  std::ifstream fin(inputFile);
  std::ofstream foutCoor(foutCoorFile);
  std::ofstream foutTemp(foutTempFile);
  std::ofstream foutCurr(foutCurrFile);
  if (!fin.is_open()) {
    exit(DATA_FILE_NOT_FOUND);
  }
  std::string x, y, z, predX, predY, predZ;
  double t;
  // 先读一个字符串类型后判断，使eof()恰好在读取完毕时返回-1
  fin >> x;
  if (fin.eof()) {
    exit(DATA_FILE_EMPTY);
  }
  fin >> y >> z >> t;
  foutCoor << x << "\t" << y << "\t" << z << std::endl;
  foutTemp << (short)(t + 0.5) << std::endl;
  foutCurr << (short)(t + 0.5) << std::endl;
  predX = x;
  fin >> x;
  while (!fin.eof()) {
    predY = y;
    predZ = z;
    fin >> y >> z >> t;
    foutCoor << (x == predX ? "*" : x) << "\t" << (y == predY ? "*" : y) << "\t"
             << (z == predZ ? "*" : z) << std::endl;
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

void UpdateTempField(std::string inputFile, const char *outputDir, int ts) {
  std::string foutTempFile = std::to_string(ts).insert(0, "/"),
              fCurrFile = CurrFilename;
  foutTempFile.insert(0, outputDir);
  fCurrFile.insert(0, outputDir);
  std::vector<short> pred;
  std::ifstream finCurr(fCurrFile);
  short predT;
  finCurr >> predT;
  while (!finCurr.eof()) {
    pred.push_back(predT);
    finCurr >> predT;
  }
  finCurr.close();
  std::ifstream fin(inputFile);
  std::ofstream foutTemp(foutTempFile, std::ios::app);
  std::ofstream foutCurr(fCurrFile, std::ios::trunc);
  std::string x, y, z;
  double t;
  short currT;
  int r = 0;
  fin >> x;
  while (!fin.eof()) {
    fin >> y >> z >> t;
    currT = (short)(t + 0.5);
    if (currT != pred[r]) {
      foutTemp << r << "\t" << currT << std::endl;
    }
    foutCurr << currT << std::endl;
    r++;
    fin >> x;
  }
  fin.close();
  foutTemp.close();
  foutCurr.close();
}
