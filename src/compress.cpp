#include "compress.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "exit_status.h"
#include "user_defined.hpp"
#include "vector3.hpp"

const std::string CoorFilename = "/coor.txt"; /* 体积元坐标 */
const std::string QSSFilename =
    "/QSS.txt"; /* 准稳态零状态下搅拌头坐标系温度场 */

/**
 * 读入准稳态零状态数据帧，初始化准稳态下搅拌头坐标系温度场
 * 参数表：输入目录、输出目录、准稳态零状态时间步、存储搅拌头坐标系温度场的地址
 */
void InitQuasiSteadyStateMap(const char *, const char *, int,
                             std::map<V3, short> &);

/**
 * 读入第1帧数据，初始化体积元的坐标和初始温度
 * 参数表：第1帧数据文件路径、输出目录、存储温度场的地址
 */
void Init(std::string, const char *, std::vector<short> &);

/**
 * 读入准稳态数据帧，更新温度场
 * 参数表：当前数据文件路径、输出目录、当前时间步、存储温度场的地址、存储搅拌头坐标系温度场的地址
 */
void UpdateQuasiSteadyState(std::string, const char *, int,
                            std::vector<short> &, const std::map<V3, short> &);

/**
 * 读入非稳态数据帧，更新温度场
 * 参数表：当前数据文件路径、输出目录、当前时间步、存储温度场的地址
 */
void UpdateNonSteadyState(std::string, const char *, int, std::vector<short> &);

void Compress(const char *inputDir, const char *outputDir) {
  std::vector<short> pred;    /* 每个时间步压缩完毕存储温度场 */
  std::map<V3, short> QSSMap; /* 搅拌头坐标系温度场零状态 */
  int n = GetNumOfTimeSteps(), lo, mi, hi;
  GetQuasiSteadyStateInterval(lo, mi, hi);
  InitQuasiSteadyStateMap(inputDir, outputDir, mi, QSSMap);
  for (int i = 0; i < n; i++) {
    std::string inputFile = GetFilename(i).insert(0, "/");
    inputFile.insert(0, inputDir);
    if (i == 0) {
      Init(inputFile, outputDir, pred);
    } else if (i >= lo && i < mi || i > mi && i < hi) {
      UpdateQuasiSteadyState(inputFile, outputDir, i, pred, QSSMap);
    } else if (i < lo || i >= hi) {
      UpdateNonSteadyState(inputFile, outputDir, i, pred);
    }
  }
}

void InitQuasiSteadyStateMap(const char *inputDir, const char *outputDir,
                             int mi, std::map<V3, short> &QSSMap) {
  double x0, y0, z0;
  GetCurrentWeld(mi, x0, y0, z0);
  std::string inputFile = GetFilename(mi).insert(0, "/");
  inputFile.insert(0, inputDir);
  std::string outputFile = QSSFilename;
  outputFile.insert(0, outputDir);
  std::ifstream fin(inputFile);
  std::ofstream fout(outputFile);
  if (!fin.is_open()) {
    exit(DATA_FILE_NOT_FOUND);
  }
  double x, y, z, t;
  fin >> x;
  if (fin.eof()) {
    exit(DATA_FILE_EMPTY);
  }
  while (!fin.eof()) {
    fin >> y >> z >> t;
    V3 v(x - x0, y - y0, z - z0);
    QSSMap[v] = (short)(t + 0.5);
    fout << v.x << "\t" << v.y << "\t" << v.z << "\t" << QSSMap[v] << std::endl;
    fin >> x;
  }
  fin.close();
  fout.close();
}

void Init(std::string inputFile, const char *outputDir,
          std::vector<short> &pred) {
  std::string foutCoorFile = CoorFilename, foutTempFile = "/0";
  foutCoorFile.insert(0, outputDir);
  foutTempFile.insert(0, outputDir);
  std::ifstream fin(inputFile);
  std::ofstream foutCoor(foutCoorFile);
  std::ofstream foutTemp(foutTempFile);
  if (!fin.is_open()) {
    exit(DATA_FILE_NOT_FOUND);
  }
  std::string x, y, z, predX, predY, predZ;
  double t;
  short currT;
  // 先读一个字符串类型后判断，使eof()恰好在读取完毕时返回-1
  fin >> x;
  if (fin.eof()) {
    exit(DATA_FILE_EMPTY);
  }
  fin >> y >> z >> t;
  foutCoor << x << "\t" << y << "\t" << z << std::endl;
  currT = (short)(t + 0.5);
  foutTemp << currT << std::endl;
  pred.push_back(currT);
  predX = x;
  fin >> x;
  while (!fin.eof()) {
    predY = y;
    predZ = z;
    fin >> y >> z >> t;
    foutCoor << (x == predX ? "*" : x) << "\t" << (y == predY ? "*" : y) << "\t"
             << (z == predZ ? "*" : z) << std::endl;
    foutTemp << currT << std::endl;
    pred.push_back(currT);
    predX = x;
    fin >> x;
  }
  fin.close();
  foutCoor.close();
  foutTemp.close();
}

void UpdateQuasiSteadyState(std::string inputFile, const char *outputDir,
                            int ts, std::vector<short> &pred,
                            const std::map<V3, short> &QSSMap) {
  std::string foutTempFile = std::to_string(ts).insert(0, "/");
  foutTempFile.insert(0, outputDir);
  std::ifstream fin(inputFile);
  std::ofstream fout(foutTempFile);
  double x, y, z, t, x0, y0, z0;
  short currT;
  GetCurrentWeld(ts, x0, y0, z0);
  for (int i = 0; i < pred.size(); i++) {
    fin >> x >> y >> z >> t;
    currT = (short)(t + 0.5);
    V3 v(x - x0, y - y0, z - z0);
    auto j = QSSMap.find(v);
    // 输出的情形：1. 搜索到且在搅拌头坐标系温度变化；2. 没有搜索到且温度变化
    if (j != QSSMap.end() && j->second != pred[i] ||
        j == QSSMap.end() && currT != pred[i]) {
      fout << i << "\t" << currT << std::endl;
    }
    if (currT != pred[i]) {
      pred[i] = currT;
    }
  }
  fin.close();
  fout.close();
}

void UpdateNonSteadyState(std::string inputFile, const char *outputDir, int ts,
                          std::vector<short> &pred) {
  std::string foutTempFile = std::to_string(ts).insert(0, "/");
  foutTempFile.insert(0, outputDir);
  std::ifstream fin(inputFile);
  std::ofstream fout(foutTempFile);
  std::string x, y, z;
  double t;
  short currT;
  for (int i = 0; i < pred.size(); i++) {
    fin >> x >> y >> z >> t;
    currT = (short)(t + 0.5);
    if (currT != pred[i]) {
      fout << i << "\t" << currT << std::endl;
      pred[i] = currT;
    }
  }
  fin.close();
  fout.close();
}
