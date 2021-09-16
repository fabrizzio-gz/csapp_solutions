#include <stdio.h>
#include <assert.h>

unsigned srl (unsigned x, int k) {
  /* Perform shift arithmetically */
  unsigned xsra = (int) x >> k;
  /* Most significant bit is 0 */
  if ((int) x >= 0)
    return xsra;
  /* mask = 11100000,  -1 = 111...1 */
  unsigned mask = -1 << (sizeof(int) * 8) - k;
  /* 0 region of the mask will left bits unchanged */
  return xsra ^ mask;
}


int sra(int x, int k) {
  /* Perform shift logically */
  int xsrl = (unsigned) x >> k;
  /* Return if most significant bit is 0 */
  if (x >= 0)
    return xsrl;

  /* mask = 11100000,  -1 = 111...1 */
  unsigned mask = -1 << (sizeof(int) * 8) - k;
  /* make most significant bits = 1 and left 0 region of the mask bits unchanged */
  return xsrl | mask;
}

int main(int argc, char* argv[]) {
  unsigned test_unsigned = 0x12345678;
  int test_int = 0x12345678;

  assert(srl(test_unsigned, 4) == test_unsigned >> 4);
  assert(sra(test_int, 4) == test_int >> 4);
  
  test_unsigned = 0x87654321;
  test_int = 0x87654321;

  assert (srl (test_unsigned, 4) == test_unsigned >> 4);
  assert (sra (test_int, 4) == test_int >> 4);

  return 0;
}
