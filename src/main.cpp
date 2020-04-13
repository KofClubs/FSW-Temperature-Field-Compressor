#include <cstdlib>
#include <cstring>
#include <iostream>

#include "check.h"
#include "compress.h"
#include "decompress.h"
#include "exit_status.h"
#include "options.hpp"

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
    case 'q':
      Check(argv[i + 1], argv[i + 2]);
      i += 3;
    }
  }
  return 0;
}
