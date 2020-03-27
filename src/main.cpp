#include <cstdlib>
#include <cstring>
#include <iostream>

#include "compress.h"
#include "decompress.h"
#include "exit_status.h"
#include "options.h"

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
      CompressTempField(argv[i + 1], std::atoi(argv[i + 2]), argv[i + 3]);
      i += 4;
      break;
    case 'd':
      GetTempFieldFrames(argv[i + 1], argv[i + 2]);
      i += 3;
    }
  }
  return 0;
}
