#!/bin/bash

# 使用其他压缩器压缩命令行参数指定目录下全体文件
# 参数表：压缩器、输入目录、输出目录

for file in $(ls $2); do
  $1 $2/$file $3/$file
done
