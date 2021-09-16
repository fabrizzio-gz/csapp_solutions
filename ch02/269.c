#include <stdio.h>
#include <assert.h>
#include "show-bytes.c"

/*
 * Do rotation left shift. Assume 0 <= n < w
 * Examples when x = 0x12345678 and w = 32;
 * n = 4 -> 0x23456781
 */

unsigned rotate_left(unsigned x, int n) {
  if (n == 0)
    return x;
  
  unsigned left_bits = x << n;
  unsigned right_bits = x >> (sizeof(int)*8 - n);
  return left_bits + right_bits;
}


int main(int argc, char* argv[]) {
  assert(rotate_left(0x12345678, 4) == 0x23456781);
  assert(rotate_left(0x12345678, 20) == 0x67812345);
  return 0;
}
