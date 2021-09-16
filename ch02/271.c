#include <stdio.h>
#include <assert.h>

/* Declaration of data type where 4 bytes are packed
   into an unsigned */
typedef unsigned packed_t;

/* Extract byte from word. Return as signed integer */
int xbyte(packed_t word, int bytenum) {
  /* Left shift of relevant byte to the leftmost.
     Followed by arithmetic right shift to the 1st byte */
  int bits = ((int) word << ((sizeof(int)-1)*8 - bytenum*8)) >> ((sizeof(int) - 1) * 8);
  return bits;
}

int main(int argc, char* argv[]) {
  assert(xbyte(0xAABBCCDD, 1) == 0xFFFFFFCC);
  assert(xbyte(0x00112233, 2) == 0x11);
  return 0;
}
