#!/usr/bin/python

# 统计命令行参数指定目录下全体文件总行数

import os
import sys

dirs = os.listdir(sys.argv[1])
os.chdir(sys.argv[1])
result = 0
for file in dirs:
    fin = open(file)
    for line in fin.readlines():
        result = result + 1
print("行总数：", result)
