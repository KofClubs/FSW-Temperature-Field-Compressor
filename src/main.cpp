#include <cstring>
#include <iostream>

#include "options.h"
#include "compress.h"
#include "decompress.h"
#include "exit_status.h"

int main(int argc, char **argv)
{
	// 不建议调用getopt函数，原因是它不能在Windows下工作
	if (argc == 1)
	{
		PrintHelpInfo();
	}
	int i = 1;
	while (i < argc)
	{
		char opt = GetOpt(argv[i]);
		switch (opt)
		{
		case 'h':
			PrintHelpInfo();
			i++;
			break;
		case 'v':
			PrintVerInfo();
			i++;
			break;
		case 'i':
			InitTempField(argv[i + 1], argv[i + 2]);
			i += 3;
			break;
		case 'c':
			UpdateTempField(argv[i + 1], argv[i + 2]);
			i += 3;
			break;
		case 'd':
			GetTempFieldFrames(argv[i + 1], argv[i + 2]);
			i += 3;
		}
	}
	return 0;
}