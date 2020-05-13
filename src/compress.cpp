#include "compress.h"

#include <cmath>
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

// 更新时间步向量过程中使用的温度精度
#define QSS_PREC 1.0
// 被用来判别准稳态的体元集合到搅拌头轴线距离的上界
#define SUP_R 0.020
// 当搅拌头坐标系下温度相同的体元数目跟全体体元数目的比不小于这个阈值时，认为进入准稳态
#define QSS_SD_TH 8.0
// 准稳态子区间数目，等效于零状态数目
#define NUM_OF_QSS_SUBINR 8

/**
 * 输入文件，输出文件：坐标、化整温差、温度变化比特图、体元数目
 */
std::string inputFile, coorFile, tempFile, bitmapFile, numOfVoxelFile;
std::vector<int> tSVector; /* 时间步向量 */
std::vector<T_T> pred;     /* 工件坐标系温度场 */
std::map<V3, T_T> qSSMap; /* 准稳态零状态下搅拌头坐标系温度场 */

/**
 * 读入第1帧数据，初始化体积元的坐标和初始温度
 */
void Init();

/**
 * 更新时间步向量，加入若干准稳态零状态
 * 参数表：输入目录、输出目录
 * 返回值：时间步向量是否被更新
 */
void UpdateTSVector(const char *&, const char *&);

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
  tSVector = GetTimeStepsVector();
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
          UpdateTSVector(inputDir, outputDir);
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
  fin.close();
  foutCoor.close();
  foutTemp.close();
  foutNOV << numOfVoxel << std::endl;
  foutNOV.close();
}

void UpdateTSVector(const char *&inputDir, const char *&outputDir) {
  std::string qSSFile =
      GetFilename(tSVector[tSVector.size() - 2] - 1).insert(0, "/");
  qSSFile.insert(0, inputDir);
  std::ifstream finQSS(qSSFile);
  double x, y, z, t, x0, y0, z0;
  GetCurrentWeld(tSVector[tSVector.size() - 2] - 1, x0, y0, z0);
  for (int i = 0; i < pred.size(); i++) {
    finQSS >> x >> y >> z >> t;
    qSSMap[V3(x - x0, y - y0, z - z0)] = T_T(t, QSS_PREC);
  }
  finQSS.close();
  for (int i = tSVector[1]; i < tSVector[tSVector.size() - 2] - 1; i++) {
    std::string iFile = GetFilename(i).insert(0, "/");
    iFile.insert(0, inputDir);
    GetCurrentWeld(i, x0, y0, z0);
    std::ifstream fin(iFile);
    int n = 0;
    double sd = 0;
    for (int j = 0; j < pred.size(); j++) {
      fin >> x >> y >> z >> t;
      if (std::pow(std::pow(x - x0, 2) + std::pow(y - y0, 2), 0.5) <= SUP_R) {
        auto k = qSSMap.find(V3(x - x0, y - y0, z - z0));
        if (k != qSSMap.end()) {
          n++;
          sd += std::pow(t - k->second.getTemp(), 2);
        }
      }
    }
    if (n > 0) {
      sd = std::pow(sd / n, 0.5);
      if (sd < QSS_SD_TH) {
        tSVector.erase(tSVector.begin() + 1);
        for (int j = 1; j <= NUM_OF_QSS_SUBINR; j++) {
          tSVector.insert(
              tSVector.begin() + 1,
              std::lround((double)(j * i + (NUM_OF_QSS_SUBINR - j) *
                                               tSVector[tSVector.size() - 2]) /
                          NUM_OF_QSS_SUBINR));
        }
        std::string tSVFile("/tsv.txt");
        tSVFile.insert(0, outputDir);
        std::ofstream fout(tSVFile);
        for (auto j : tSVector) {
          fout << j << std::endl;
        }
        fout.close();
        break;
      }
    }
    fin.close();
  }
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
    if (j != qSSMap.end() &&
            pred[i] != j->second /* 搜索到且在搅拌头坐标系温度变化 */
        || j == qSSMap.end() && intTempDiff != 0 /* 没有搜索到且温度变化 */) {
      foutTemp << intTempDiff << std::endl;
      bm.set(i);
    }
  }
  bm.dump(bitmapFile.c_str());
  fin.close();
  foutTemp.close();
}
