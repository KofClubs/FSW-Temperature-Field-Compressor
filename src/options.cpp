#include "options.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "exit_status.h"

char GetOpt(char *arg)
{
	if (arg[0] == '-')
	{
		switch (arg[1])
		{
		case 'h':
			return 'h';
		case 'v':
			return 'v';
		case 'i':
			return 'i';
		case 'c':
			return 'c';
		case 'd':
			return 'd';
		default:
			std::cout << "Invalid option!" << std::endl;
			exit(INVALID_OPTION);
		}
	}
	else
	{
		std::cout << "Invalid argument vector format!" << std::endl;
		exit(INVALID_ARGV_FORMAT);
	}
}

void PrintHelpInfo()
{
	std::cout << "用法: ftfc [-选项] [参数]" << std::endl;
	std::cout << "选项包括且限于:" << std::endl;
	std::cout << "-h\t在命令行打印帮助信息" << std::endl;
	std::cout << "-v\t在命令行打印版本信息" << std::endl;
	std::cout << "-i [文件名]\t指定第1个数据帧，获得体积元坐标和零时刻温度场" << std::endl;
	std::cout << "-c [文件名]\t顺序指定其他数据帧，记录其他时刻温度场" << std::endl;
	std::cout << "-d [文件名] [文件名] [目录]\t指定压缩获得的坐标文件、温度场文件和解压目录，顺序重现数据帧" << std::endl;
}

void PrintVerInfo()
{
	std::cout << "ftfc - FSW Temperature Field Compressor" << std::endl;
	std::cout << "版本：0.1" << std::endl;
}