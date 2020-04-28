#!/bin/bash

# 6pack批处理器
# 6pack详见：https://github.com/ariya/FastLZ
# 参数表：单文件命令、输入目录、输出目录

for file in $(ls $2); do
  $1 $2/$file $3/$file
done
