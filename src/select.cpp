#include "select.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "exit_status.h"
#include "user_defined.h"
#include "vector3.h"

// 建议稍大于相邻体元最短距离的0.8660倍
#define EPSILON 0.8661e-4

void Select(const char *inputDir, const char *outputFile, double x0, double y0,
            double z0) {
  std::vector<int> tSVector = GetTimeStepsVector();
  std::ofstream fout(outputFile);
  int lineCount;
  for (int i = tSVector.front(); i < tSVector.back(); i++) {
    std::string inputFile = GetFilename(i).insert(0, "/");
    inputFile.insert(0, inputDir);
    std::ifstream fin(inputFile);
    if (!fin.is_open()) {
      exit(DATA_FILE_NOT_FOUND);
    }
    V3 v0(x0, y0, z0);
    double x, y, z, m;
    std::string t, t0;
    if (i == tSVector.front()) {
      fin >> x;
      if (fin.eof()) {
        exit(DATA_FILE_EMPTY);
      }
      fin >> y >> z >> t;
      m = v0.mod(V3(x, y, z));
      t0 = t;
      int count = 0;
      fin >> x;
      while (!fin.eof()) {
        fin >> y >> z >> t;
        count++;
        if (v0.mod(V3(x, y, z)) < m) /* 更新条件：找到更近的体元 */ {
          m = v0.mod(V3(x, y, z));
          t0 = t;
          lineCount = count;
        }
        fin >> x;
      }
      if (m >= EPSILON) {
        exit(VOXEL_NOT_FOUND);
      }
    } else {
      for (int j = 0; j <= lineCount; j++) {
        fin >> x >> y >> z >> t0;
      }
    }
    fout << i << "\t" << t0 << std::endl;
    fin.close();
  }
  fout.close();
}
