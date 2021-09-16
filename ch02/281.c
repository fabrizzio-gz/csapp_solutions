#include <stdio.h>
#include <assert.h>
#include "show-bytes.c"


int w = sizeof(int)<<3;

int A(int k) {
  return (unsigned) -1 >> k << k;
}


int B(int k, int j) {
  return (unsigned) -1 >> (w - k) << j;
}

int main(int argc, char* argv[]) {
  assert(A(8) == 0xFFFFFF00);
  assert(B(16, 8) == 0x00FFFF00);
  return 0;
}
