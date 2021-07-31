#include <stdio.h>
#include "show-bytes.c"

/* Gets least significant byte of x and rest of 
   y unchanged */
int main() {
  int x = 0x89ABCDEF;
  int y = 0x76543210;

  int z = (x & 0xff) + (y & (1 - 0xff));
  show_int(x);
  show_int(y);
  show_int(z);
}
