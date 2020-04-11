#!/usr/bin/python

# 打印游程数目

import os
import sys

dirs = os.listdir(sys.argv[1])
os.chdir(sys.argv[1])
count = 0
for file in dirs:
    header = True
    predChar = '0'
    fin = open(file)
    for line in fin:
        for char in line:
            if header:
                predChar = char
                header = False
            elif predChar != char:
                count = count + 1
                predChar = char
print(count)
