#include "decompress.h"

#include <cstddef>
#include <fstream>
#include <map>
#include <string>

#include "bitmap.h"
#include "user_defined.h"
#include "vector3.h"

void Decompress(const char *inputDir, const char *outputDir) {
  int numOfVoxel;
  std::string numOfVoxelFile("/nov.txt"), coorFile("/coor.txt");
  numOfVoxelFile.insert(0, inputDir);
  coorFile.insert(0, inputDir);
  std::ifstream finNOV(numOfVoxelFile);
  std::ifstream finCoor(coorFile);
  finNOV >> numOfVoxel;
  finNOV.close();
  std::string **coor = new std::string *[numOfVoxel];
  for (int i = 0; i < numOfVoxel; i++) {
    coor[i] = new std::string[3];
  }
  std::string x, y, z;
  for (int i = 0; i < numOfVoxel; i++) {
    finCoor >> x >> y >> z;
    if (i == 0) {
      coor[i][0] = x;
      coor[i][1] = y;
      coor[i][2] = z;
    } else {
      coor[i][0] = (x == "*" ? coor[i - 1][0] : x);
      coor[i][1] = (y == "*" ? coor[i - 1][1] : y);
      coor[i][2] = (z == "*" ? coor[i - 1][2] : z);
    }
  }
  finCoor.close();
  short t;
  std::vector<short> pred;
  std::vector<int> tSVector = GetTimeStepsVector();
  std::map<V3, short> qSSMap;
  for (int i = 0; i < tSVector.size() - 1; i++) {
    for (int j = tSVector[i]; j < tSVector[i + 1]; j++) {
      std::string tempFile = std::to_string(j).insert(0, "/t-");
      tempFile.insert(0, inputDir);
      std::string bitmapFile = std::to_string(j).insert(0, "/b-");
      bitmapFile.insert(0, inputDir);
      std::string outputFile = GetFilename(j).insert(0, "/");
      outputFile.insert(0, outputDir);
      std::ifstream finTemp(tempFile);
      std::ofstream fout(outputFile);
      if (i == 0 || i == tSVector.size() - 2) {
        if (j == tSVector.front()) {
          for (int k = 0; k < numOfVoxel; k++) {
            finTemp >> t;
            fout << coor[k][0] << "\t" << coor[k][1] << "\t" << coor[k][2]
                 << "\t" << t << std::endl;
            pred.push_back(t);
          }
        } else {
          BM bm(bitmapFile.c_str(), numOfVoxel);
          for (int k = 0; k < numOfVoxel; k++) {
            if (bm.check(k)) {
              finTemp >> t;
              pred[k] += t;
            }
            fout << coor[k][0] << "\t" << coor[k][1] << "\t" << coor[k][2]
                 << "\t" << pred[k] << std::endl;
          }
        }
      } else {
        double x0, y0, z0;
        GetCurrentWeld(j, x0, y0, z0);
        BM bm(bitmapFile.c_str(), numOfVoxel);
        if (j == tSVector[i]) {
          qSSMap.clear();
          for (int k = 0; k < numOfVoxel; k++) {
            if (bm.check(k)) {
              finTemp >> t;
              pred[k] += t;
            }
            fout << coor[k][0] << "\t" << coor[k][1] << "\t" << coor[k][2]
                 << "\t" << pred[k] << std::endl;
            V3 v(std::stod(coor[k][0]) - x0, std::stod(coor[k][1]) - y0,
                 std::stod(coor[k][2]) - z0);
            qSSMap[v] = pred[k];
          }
        } else {
          for (int k = 0; k < numOfVoxel; k++) {
            V3 v(std::stod(coor[k][0]) - x0, std::stod(coor[k][1]) - y0,
                 std::stod(coor[k][2]) - z0);
            auto l = qSSMap.find(v);
            if (l != qSSMap.end() && !bm.check(k)) {
              pred[k] = l->second;
            } else if (bm.check(k)) {
              finTemp >> t;
              pred[k] += t;
            }
            fout << coor[k][0] << "\t" << coor[k][1] << "\t" << coor[k][2]
                 << "\t" << pred[k] << std::endl;
          }
        }
      }
      finTemp.close();
      fout.close();
    }
  }
  for (int i = 0; i < numOfVoxel; i++) {
    delete[] coor[i];
    coor[i] = NULL;
  }
  delete[] coor;
  coor = NULL;
}
