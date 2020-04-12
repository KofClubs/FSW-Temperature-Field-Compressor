/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2019. All rights reserved.
 ******************************************************************************************/

#include "bitmap.h"

#include <cstddef>
#include <cstdio>
#include <cstring>

inline void Bitmap::init(int n) {
  M = new char[N = n + 7 >> 3];
  std::memset(M, 0, N);
}

Bitmap::Bitmap(int n = 1) { init(n); }

Bitmap::Bitmap(const char *inputFile, int n = 1) {
  init(n);
  FILE *pf = fopen(inputFile, "r");
  fread(M, sizeof(char), N, pf);
  fclose(pf);
}

Bitmap::~Bitmap() {
  delete[] M;
  M = NULL;
}

void Bitmap::set(int r) { M[r >> 3] |= (0x80 >> (r & 0x07)); }

void Bitmap::unset(int r) { M[r >> 3] &= ~(0x80 >> (r & 0x07)); }

bool Bitmap::check(int r) { return M[r >> 3] & (0x80 >> (r & 0x07)); }

void Bitmap::dump(const char *outputFile) {
  FILE *pf = fopen(outputFile, "w");
  fwrite(M, sizeof(char), N, pf);
  fclose(pf);
}
