#include <cstdlib>
#include <cstring>
#include <iostream>

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
    case 'h':
      PrintHelpInfo();
      i++;
      break;
    case 'v':
      PrintVerInfo();
      i++;
      break;
    case 'c':
      Compress(argv[i + 1], argv[i + 2]);
      i += 3;
      break;
    case 'd':
      Decompress(argv[i + 1], argv[i + 2]);
      i += 3;
      break;
    case 't':
      Test(argv[i + 1], argv[i + 2]);
      i += 3;
    case 's':
      Select(argv[i + 1], argv[i + 2], std::atof(argv[i + 3]),
             std::atof(argv[i + 4]), std::atof(argv[i + 5]));
      i += 6;
    }
  }
  return 0;
}
