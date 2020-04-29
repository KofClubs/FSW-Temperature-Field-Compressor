#include "test.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "temperature.h"
#include "user_defined.h"

void PrintHeader();

void Test(const char *firstDir, const char *secondDir) {
  std::vector<int> tSVector = GetTimeStepsVector();
  bool flag = false; /* 错误存在性标记 */
  for (int i = tSVector.front(); i < tSVector.back(); i++) {
    std::string firstFile = GetFilename(i).insert(0, "/");
    std::string secondFile = firstFile;
    firstFile.insert(0, firstDir);
    secondFile.insert(0, secondDir);
    std::ifstream fin1(firstFile);
    std::ifstream fin2(secondFile);
    std::string x1, y1, z1, x2, y2, z2;
    double p, t1, t2;
    int lineCount = 1;
    fin1 >> x1;
    while (!fin1.eof()) {
      fin1 >> y1 >> z1 >> t1;
      fin2 >> x2 >> y2 >> z2 >> t2;
      if (x1 != x2 || y1 != y2 || z1 != z2) {
        if (!flag) {
          PrintHeader();
          flag = true;
        }
        std::cout << "坐标"
                  << "\t" << GetFilename(i) << "\t" << lineCount << std::endl;
      } else {
        p = GetPrec(std::stod(x1), std::stod(y1), std::stod(z1));
        T_T tt1(t1, p);
        T_T tt2(t2, p);
        if (tt1 != tt2) {
          if (!flag) {
            PrintHeader();
            flag = true;
          }
          std::cout << "温度"
                    << "\t" << GetFilename(i) << "\t" << lineCount << std::endl;
        }
      }
      fin1 >> x1;
      lineCount++;
    }
    fin1.close();
    fin2.close();
  }
  if (!flag) {
    std::cout << "测试完毕，解压输出跟压缩输入一致！" << std::endl;
  }
}

void PrintHeader() {
  std::cout << "解压输出跟压缩输入不一致！" << std::endl;
  std::cout << "类型"
            << "\t"
            << "文件名"
            << "\t"
            << "行数" << std::endl;
}
