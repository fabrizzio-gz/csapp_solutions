#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

int signed_high_prod(int x, int y) {
  int64_t mul = (int64_t) x * y;
  return mul >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y) {

  
  int p = (int) unsigned_high_prod((unsigned) x, (unsigned)y);
  if (x < 0) /*x_{w-1} = 1*/
  
    p -= y;
  if (y < 0) /*y_{w-1} = 1*/
    p -= x;
  return p;
  /* Get x and y most sig bits */
  int x_ms = x >> (sizeof(int)*8 - 1);
  int y_ms = y >> (sizeof(int)*8 - 1);

  /* x.y = x.y(signed) + x_ms * y + y_ms * x + 2 * x_ms * y_ms */

  return signed_high_prod(x, y) - (x_ms && y) - (y_ms && x); //+ (x_ms && y_ms && 2);
}


unsigned anthoer_unsigned_high_prod(unsigned x, unsigned y)
{
	uint64_t mul = (uint64_t) x * y;
	return mul >> 32;
}

int main(void)
{
  unsigned x = 0x12345678;
  unsigned y = 0xFFFFFFFF;
	
  assert(unsigned_high_prod(x, y) == anthoer_unsigned_high_prod(x, y));
	
  return 0;
}
