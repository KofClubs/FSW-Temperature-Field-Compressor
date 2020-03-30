#include "compress.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "exit_status.h"
#include "user_defined.hpp"
#include "vector3.hpp"

/**
 * 读入第1帧数据，初始化体积元的坐标和初始温度
 * 参数表：第1帧数据文件路径、坐标输出文件、零时刻温度场输出文件、存储温度场的地址
 */
void Init(std::string, std::string, std::string, std::vector<short> &);

/**
 * 读入准稳态零状态数据帧，初始化准稳态下搅拌头坐标系温度场
 * 参数表：当前数据文件路径、输出文件、当前时间步、存储温度场的地址、存储搅拌头坐标系温度场的地址
 */
void InitQuasiSteadyState(std::string, std::string, int, std::vector<short> &,
                          std::map<V3, short> &);

/**
 * 读入非稳态数据帧，更新温度场
 * 参数表：当前数据文件路径、输出文件、当前时间步、存储温度场的地址
 */
void UpdateNonSteadyState(std::string, std::string, int, std::vector<short> &);

/**
 * 读入准稳态数据帧，更新温度场
 * 参数表：当前数据文件路径、输出文件、当前时间步、存储温度场的地址、存储搅拌头坐标系温度场的地址
 */
void UpdateQuasiSteadyState(std::string, std::string, int, std::vector<short> &,
                            const std::map<V3, short> &);

void Compress(const char *inputDir, const char *outputDir) {
  std::vector<short> pred; /* 每个时间步压缩完毕存储温度场 */
  std::vector<int> tSVector = GetTimeStepsVector(); /* 时间步向量 */
  std::map<V3, short> qSSMap; /* 搅拌头坐标系温度场零状态 */
  for (int i = 0; i < tSVector.size() - 1; i++) {
    for (int j = tSVector[i]; j < tSVector[i + 1]; j++) {
      std::string inputFile = GetFilename(j).insert(0, "/");
      inputFile.insert(0, inputDir);
      std::string outputFile = std::to_string(j).insert(0, "/");
      outputFile.insert(0, outputDir);
      if (i == 0 || i == tSVector.size() - 2) /* 非稳态 */ {
        if (j == 0) {
          std::string coorFile = GetCoorFilename().insert(0, "/");
          coorFile.insert(0, outputDir);
          Init(inputFile, coorFile, outputFile, pred);
        } else {
          UpdateNonSteadyState(inputFile, outputFile, j, pred);
        }
      } else /* 准稳态 */ {
        if (j == tSVector[i]) {
          InitQuasiSteadyState(inputFile, outputFile, j, pred, qSSMap);
        } else {
          UpdateQuasiSteadyState(inputFile, outputFile, j, pred, qSSMap);
        }
      }
    }
  }
}

void Init(std::string inputFile, std::string coorFile, std::string outputFile,
          std::vector<short> &pred) {
  std::ifstream fin(inputFile);
  std::ofstream foutCoor(coorFile);
  std::ofstream foutTemp(outputFile);
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

void InitQuasiSteadyState(std::string inputFile, std::string outputFile, int ts,
                          std::vector<short> &pred,
                          std::map<V3, short> &qSSMap) {
  double x0, y0, z0;
  GetCurrentWeld(ts, x0, y0, z0);
  std::ifstream fin(inputFile);
  std::ofstream fout(outputFile);
  double x, y, z, t;
  short currT;
  int i = 0;
  qSSMap.clear();
  fin >> x;
  while (!fin.eof()) {
    fin >> y >> z >> t;
    currT = (short)(t + 0.5);
    if (currT != pred[i]) {
      fout << i << "\t" << currT << std::endl;
      pred[i] = currT;
    }
    V3 v(x - x0, y - y0, z - z0);
    qSSMap[v] = currT;
    fin >> x;
    i++;
  }
  fin.close();
  fout.close();
}

void UpdateNonSteadyState(std::string inputFile, std::string outputFile, int ts,
                          std::vector<short> &pred) {
  std::ifstream fin(inputFile);
  std::ofstream fout(outputFile);
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

void UpdateQuasiSteadyState(std::string inputFile, std::string outputFile,
                            int ts, std::vector<short> &pred,
                            const std::map<V3, short> &qSSMap) {
  std::ifstream fin(inputFile);
  std::ofstream fout(outputFile);
  double x, y, z, t, x0, y0, z0;
  short currT;
  GetCurrentWeld(ts, x0, y0, z0);
  for (int i = 0; i < pred.size(); i++) {
    fin >> x >> y >> z >> t;
    currT = (short)(t + 0.5);
    V3 v(x - x0, y - y0, z - z0);
    auto j = qSSMap.find(v);
    // 输出的情形：1. 搜索到且在搅拌头坐标系温度变化；2. 没有搜索到且温度变化
    if (j != qSSMap.end() && currT != j->second ||
        j == qSSMap.end() && currT != pred[i]) {
      fout << i << "\t" << currT << std::endl;
    }
    if (currT != pred[i]) {
      pred[i] = currT;
    }
  }
  fin.close();
  fout.close();
}
