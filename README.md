FSW-Temperature-Field-Compressor
================================

综合论文训练，搅拌摩擦焊工件温度场仿真数据压缩器

使用方法
--------

1.	编辑用户定义文件`user_defined.hpp`，根据注释提示实现全体函数。

2.	编译、构建、运行。

```
$ make all
$ cd bin
$ ./ftfc -h # 获得帮助
```

版本历史
--------

### 0.1.0

2020-02-10

1.	支持短参数`-hvi`。

2.	压缩Tecplot导出的单帧温度场数据。

### 0.2.0

2020-03-27

1.	压缩多帧温度场数据，输入格式是(x, y, z, T)，每个时间步的存储成单个文件。

2.	新增了用户定义文件`user_defined.hpp`，详见注释。

3.	新增了短参数`-c`。

4.	移除了短参数`-i`。

### 0.3.0

2020-03-29

1.	根据搅拌摩擦焊存在准稳态过程，支持用户定义1个零状态。

### 0.3.1

2020-03-29

1.	优化了代码，丰富了注释。

### 0.3.2

2020-03-30

1.	支持用户定义坐标文件名。

2.	支持用户定义多个搅拌摩擦焊准稳态的零状态。

3.	优化了代码，丰富了注释。

### 0.3.3

2020-04-07

1.	新增了脚本：字符频数统计器`char_counter.py`、批量压缩器`compressor.sh`、行计数器`line_counter.py`，详见注释。
2.	优化了代码。

### 0.3.4

2020-04-11

1.	输出文件被改成记录温差。
2.	新增了脚本：游程计数器`rlc_counter.py`，详见注释。
3.	优化了代码。
