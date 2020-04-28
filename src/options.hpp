#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <cstdlib>
#include <iostream>

#include "exit_status.h"

/**
 * 获得当前参数的选项
 * 参数表：参数
 * 返回值：选项
 */
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
    case 'q':
      return 'q';
    default:
      std::cout << "Invalid option!" << std::endl;
      exit(INVALID_OPTION);
    }
  } else {
    std::cout << "Invalid argument vector format!" << std::endl;
    exit(INVALID_ARGV_FORMAT);
  }
}

/**
 * 打印帮助信息
 */
void PrintHelpInfo() {
  std::cout << "ftfc - FSW Temperature Field Compressor" << std::endl;
  std::cout << "综合论文训练，搅拌摩擦焊工件温度场仿真数据压缩器" << std::endl;
  std::cout << "Copyright (C) 张展鹏" << std::endl;
  std::cout << "用法: ftfc [-选项] [参数表]" << std::endl;
  std::cout << "选项包括且限于:" << std::endl;
  std::cout << "-h\n\t在命令行打印帮助信息" << std::endl;
  std::cout << "-v\n\t在命令行打印版本信息" << std::endl;
  std::cout << "-c [目录] [目录]\t压缩器\n\t指定输入目录、输出目录"
            << std::endl;
  std::cout << "-d [目录] [目录]\t解压器\n\t指定压缩输出目录、解压输出目录"
            << std::endl;
  std::cout << "-q [目录] [目录]\t校验器\n\t压缩输入目录、解压输出目录"
            << std::endl;
}

/**
 * 打印版本信息
 */
void PrintVerInfo() {
  std::cout << "ftfc - FSW Temperature Field Compressor" << std::endl;
  std::cout << "综合论文训练，搅拌摩擦焊工件温度场仿真数据压缩器" << std::endl;
  std::cout << "Copyright (C) 张展鹏" << std::endl;
  std::cout << "版本：0.6.0" << std::endl;
  std::cout << "使用选项-h获得帮助" << std::endl;
}

#endif
