#include <stdio.h>
#include <assert.h>

/* Return 1 when any odd bit of x equals 1; 0 otherwise.
   Assume w = 32 */
int any_odd_one(unsigned x) {
  /* 0xaa = 1010 1010 */
  /* will return 0 only if all odd bits of x are 0 */
  return !(x & ~0xaaaaaaaa);
}

int main(int argc, char* argv[]) {
  assert(any_odd_one(0x2));
  assert(!any_odd_one(0x4));
  return 0;
}
