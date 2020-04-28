#include "compress.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "bitmap.h"
#include "exit_status.h"
#include "temperature.h"
#include "user_defined.h"
#include "vector3.h"

/**
 * 输入文件，输出文件：坐标、化整温差、温度变化比特图、体元数目
 */
std::string inputFile, coorFile, tempFile, bitmapFile, numOfVoxelFile;
std::vector<T_T> pred;    /* 工件坐标系温度场 */
std::map<V3, T_T> qSSMap; /* 准稳态零状态下搅拌头坐标系温度场 */

/**
 * 读入第1帧数据，初始化体积元的坐标和初始温度
 */
void Init();

/**
 * 读入准稳态零状态数据帧，初始化准稳态下搅拌头坐标系温度场
 * 参数表：时间步
 */
void InitQuasiSteadyState(int);

/**
 * 读入非稳态数据帧，更新温度场
 */
void UpdateNonSteadyState();

/**
 * 读入准稳态数据帧，更新温度场
 * 参数表：时间步
 */
void UpdateQuasiSteadyState(int);

void Compress(const char *inputDir, const char *outputDir) {
  std::vector<int> tSVector = GetTimeStepsVector(); /* 时间步向量 */
  for (int i = 0; i < tSVector.size() - 1; i++) {
    for (int j = tSVector[i]; j < tSVector[i + 1]; j++) {
      inputFile = GetFilename(j).insert(0, "/");
      inputFile.insert(0, inputDir);
      tempFile = std::to_string(j).insert(0, "/t-");
      tempFile.insert(0, outputDir);
      int numOfVoxel = 0;
      if (i == 0 || i == tSVector.size() - 2) /* 非稳态 */ {
        if (j == tSVector.front()) /* 零时间步 */ {
          coorFile = "/coor.txt";
          coorFile.insert(0, outputDir);
          numOfVoxelFile = "/nov.txt";
          numOfVoxelFile.insert(0, outputDir);
          Init();
        } else {
          bitmapFile = std::to_string(j).insert(0, "/b-");
          bitmapFile.insert(0, outputDir);
          UpdateNonSteadyState();
        }
      } else /* 准稳态 */ {
        bitmapFile = std::to_string(j).insert(0, "/b-");
        bitmapFile.insert(0, outputDir);
        if (j == tSVector[i]) /* 准稳态零状态 */ {
          InitQuasiSteadyState(j);
        } else {
          UpdateQuasiSteadyState(j);
        }
      }
    }
  }
}

void Init() {
  std::ifstream fin(inputFile);
  std::ofstream foutCoor(coorFile);
  std::ofstream foutTemp(tempFile);
  std::ofstream foutNOV(numOfVoxelFile);
  if (!fin.is_open()) {
    exit(DATA_FILE_NOT_FOUND);
  }
  std::string x, y, z, predX, predY, predZ;
  double t;
  int numOfVoxel = 0;
  fin >> x;
  if (fin.eof()) {
    exit(DATA_FILE_EMPTY);
  }
  fin >> y >> z >> t;
  foutCoor << x << "\t" << y << "\t" << z << std::endl;
  pred.push_back(T_T(t, GetPrec(std::stod(x), std::stod(y), std::stod(z))));
  foutTemp << pred[numOfVoxel].getIntTemp() << std::endl;
  numOfVoxel++;
  predX = x;
  fin >> x;
  while (!fin.eof()) {
    predY = y;
    predZ = z;
    fin >> y >> z >> t;
    foutCoor << (x == predX ? "*" : x) << "\t" << (y == predY ? "*" : y) << "\t"
             << (z == predZ ? "*" : z) << std::endl;
    pred.push_back(T_T(t, GetPrec(std::stod(x), std::stod(y), std::stod(z))));
    foutTemp << pred[numOfVoxel].getIntTemp() << std::endl;
    numOfVoxel++;
    predX = x;
    fin >> x;
  }
  foutNOV << numOfVoxel << std::endl;
  fin.close();
  foutCoor.close();
  foutTemp.close();
  foutNOV.close();
}

void InitQuasiSteadyState(int ts) {
  std::ifstream fin(inputFile);
  std::ofstream foutTemp(tempFile);
  double x, y, z, t, x0, y0, z0;
  GetCurrentWeld(ts, x0, y0, z0);
  qSSMap.clear();
  BM bm(pred.size());
  for (int i = 0; i < pred.size(); i++) {
    fin >> x >> y >> z >> t;
    long long intTempDiff = pred[i].updateTemp(t);
    if (intTempDiff != 0) {
      foutTemp << intTempDiff << std::endl;
      bm.set(i);
    }
    qSSMap[V3(x - x0, y - y0, z - z0)] = pred[i];
  }
  bm.dump(bitmapFile.c_str());
  fin.close();
  foutTemp.close();
}

void UpdateNonSteadyState() {
  std::ifstream fin(inputFile);
  std::ofstream foutTemp(tempFile);
  std::string x, y, z;
  double t;
  BM bm(pred.size());
  for (int i = 0; i < pred.size(); i++) {
    fin >> x >> y >> z >> t;
    long long intTempDiff = pred[i].updateTemp(t);
    if (intTempDiff != 0) {
      foutTemp << intTempDiff << std::endl;
      bm.set(i);
    }
  }
  bm.dump(bitmapFile.c_str());
  fin.close();
  foutTemp.close();
}

void UpdateQuasiSteadyState(int ts) {
  std::ifstream fin(inputFile);
  std::ofstream foutTemp(tempFile);
  double x, y, z, t, x0, y0, z0;
  GetCurrentWeld(ts, x0, y0, z0);
  BM bm(pred.size());
  for (int i = 0; i < pred.size(); i++) {
    fin >> x >> y >> z >> t;
    long long intTempDiff = pred[i].updateTemp(t);
    auto j = qSSMap.find(V3(x - x0, y - y0, z - z0));
    // 输出的情形：1. 搜索到且在搅拌头坐标系温度变化；2. 没有搜索到且温度变化
    if (j != qSSMap.end() && pred[i] != j->second ||
        j == qSSMap.end() && intTempDiff != 0) {
      foutTemp << intTempDiff << std::endl;
      bm.set(i);
    }
  }
  bm.dump(bitmapFile.c_str());
  fin.close();
  foutTemp.close();
}
