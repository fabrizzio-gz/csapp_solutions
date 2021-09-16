#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "show-bytes.c"

int mul3div4(int x) {
  int mul3 = (x << 1) + x;
  int is_neg = x & INT_MIN;
  (is_neg && (mul3 = mul3 + (1 << 2) - 1));
  int div4 = mul3 >> 2;
  return div4;
}

int main(int argc, char* argv[]) {
  int x = 0xf2345678;
  assert(mul3div4(x) == x * 3 / 4);
  return 0;
}
