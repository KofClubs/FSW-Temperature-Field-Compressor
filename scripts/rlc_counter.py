#!/usr/bin/python3

# 打印游程数目和最长游程长度

import os
import sys

dirs = os.listdir(sys.argv[1])
os.chdir(sys.argv[1])
count = 0
maxLen = 0
for file in dirs:
    header = True
    fin = open(file)
    for line in fin:
        for char in line:
            if header:
                predChar = char
                currLen = 1
                header = False
            elif predChar == char:
                currLen = currLen + 1
            else:
                count = count + 1
                predChar = char
                if currLen > maxLen:
                    maxLen = currLen
                currLen = 1
print("游程数目：", count)
print("最长游程长度：", maxLen)
