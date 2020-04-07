#!/usr/bin/python

# 统计命令行参数指定目录下全体文件字符频数

import os
import sys


def update_dict(l, d):
    new_d = d
    for c in l:
        new_d[c] = new_d.get(c, 0) + 1
    return new_d


dirs = os.listdir(sys.argv[1])
os.chdir(sys.argv[1])
freq = dict()
for file in dirs:
    fin = open(file)
    for line in fin:
        freq = update_dict(line, freq)
sorted_freq = sorted(freq.items(), key=lambda d: d[1])
print(sorted_freq)
