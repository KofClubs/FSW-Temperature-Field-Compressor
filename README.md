FSW-Temperature-Field-Compressor
================================

综合论文训练，搅拌摩擦焊工件温度场仿真数据压缩器

使用方法
--------

编辑用户定义文件`user_defined.cpp`，根据对应头文件的注释提示实现全体函数。

编译、构建、运行。

```
$ make all
$ cd bin
$ ./ftfc -h # 获得帮助
```

使用脚本。例如使用字符频数统计器`char_counter.py` 统计某目录`MY_DIR` 下全体文件的字符频数，在`scripts` 目录下执行：

```
$ chmod +x char_counter.py
$ ./char_counter.py MY_DIR
```

在`doc`目录下阅读有关文档。

当前版本变化
------------

### 0.8.2

2020-05-07

1.	修改了准稳态建立的判别方法，改成了用搅拌头附近体元在搅拌头坐标系下标准差来判别。
