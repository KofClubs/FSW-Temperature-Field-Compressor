#ifndef __BITMAP_H__
#define __BITMAP_H__

/**
 * 比特图
 * 对任意r，第r个比特是“0”或“1”分别对应假或真
 */
class Bitmap {
private:
  char *M; /* 比特图字符串 */
  int N;   /* 字符串长度 */
private:
  void init(int); /* 内联函数，零初始化，参数表：比特数目下界 */
public:
  Bitmap(int); /* 零构造，参数表：比特数目下界 */
  Bitmap(const char *,
         int); /* 从文件拷贝构造，参数表：文件路径、比特数目下界 */
  ~Bitmap(); /* 析构 */
public:
  void set(int);           /* 置“1”，参数表：比特序数 */
  void unset(int);         /* 置“0”，参数表：比特序数 */
  bool check(int);         /* 返回第r个比特对应的真假 */
  void dump(const char *); /* 输出到文件，参数表：文件路径 */
};

typedef Bitmap BM;

#endif
