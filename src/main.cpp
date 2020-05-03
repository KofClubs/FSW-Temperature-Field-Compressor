#include <cstdlib>

#include "compress.h"
#include "decompress.h"
#include "exit_status.h"
#include "options.hpp"
#include "select.h"
#include "test.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    PrintHelpInfo();
  }
  int i = 1;
  while (i < argc) {
    char opt = GetOpt(argv[i]);
    switch (opt) {
    case 'h': /* 打印帮助信息 */
      PrintHelpInfo();
      i++;
      break;
    case 'v': /* 打印版本信息 */
      PrintVerInfo();
      i++;
      break;
    case 'c': /* 压缩 */
      if (i + 3 > argc) {
        exit(ARGV_TOO_SHORT);
      }
      Compress(argv[i + 1], argv[i + 2]);
      i += 3;
      break;
    case 'd': /* 解压 */
      if (i + 3 > argc) {
        exit(ARGV_TOO_SHORT);
      }
      Decompress(argv[i + 1], argv[i + 2]);
      i += 3;
      break;
    case 't': /* 测试 */
      if (i + 3 > argc) {
        exit(ARGV_TOO_SHORT);
      }
      Test(argv[i + 1], argv[i + 2]);
      i += 3;
      break;
    case 's': /* 查询 */
      if (i + 6 > argc) {
        exit(ARGV_TOO_SHORT);
      }
      Select(argv[i + 1], argv[i + 2], std::atof(argv[i + 3]),
             std::atof(argv[i + 4]), std::atof(argv[i + 5]));
      i += 6;
    }
  }
  return 0;
}
