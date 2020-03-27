#include "options.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "exit_status.h"

char GetOpt(char *arg) {
  if (arg[0] == '-') {
    switch (arg[1]) {
    case 'h':
      return 'h';
    case 'v':
      return 'v';
    case 'c':
      return 'c';
    case 'd':
      return 'd';
    default:
      std::cout << "Invalid option!" << std::endl;
      exit(INVALID_OPTION);
    }
  } else {
    std::cout << "Invalid argument vector format!" << std::endl;
    exit(INVALID_ARGV_FORMAT);
  }
}

void PrintHelpInfo() {
  std::cout << "用法: ftfc [-选项] [参数]" << std::endl;
  std::cout << "选项包括且限于:" << std::endl;
  std::cout << "-h\t在命令行打印帮助信息" << std::endl;
  std::cout << "-v\t在命令行打印版本信息" << std::endl;
  std::cout
      << "-c\t[目录] [自然数] [目录]\t指定数据帧目录和数据帧数目、输出目录"
      << std::endl;
  std::cout
      << "-d\t[文件名] [文件名] "
         "[目录]\t指定压缩获得的坐标文件、温度场文件和解压目录，顺序重现数据帧"
      << std::endl;
}

void PrintVerInfo() {
  std::cout << "ftfc - FSW Temperature Field Compressor" << std::endl;
  std::cout << "版本：0.2.0" << std::endl;
}
