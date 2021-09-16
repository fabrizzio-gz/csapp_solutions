#include <stdio.h>
#include "show-bytes.c"


unsigned replace_byte(unsigned x, int i, unsigned char b) {
  unsigned mask = ~(0xff << (i*8));
  return (x & mask) | (b<<(i*8));
}

int main() {
  unsigned x = 0x12345678;
  show_int(x);
  unsigned y = replace_byte(x, 2, 0xAB);
  show_int(y);
  return 0;
}
