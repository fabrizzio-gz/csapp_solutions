#include <stdio.h>
#include <assert.h>
#include "show-bytes.c"

typedef unsigned float_bits;

float_bits float_half(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = (f >> 23) & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  int is_nan = exp == 0xFF && frac != 0;
  int is_inf = exp == 0xFF && frac == 0;

  if (is_nan)
    return f;

  if (is_inf)
    return f;

  if (exp != 0) {
    /* Normalized values */
    if (exp == 1) {
      /* convert to denormalized */
      exp = 0;
      /* shift M to right and make most significant bit 1 */
      /* normalized M = 1 + denormalized M */
      frac = (frac >> 1) | 0x400000;
    } else
      /* Decrease exponent by one == divide by two */
      exp -= 1;
  } else
    /* Denormalized values */
    /* Right shift by one == divide by two */
    frac = frac >> 1;
    
  
  return (sign << 31) | (exp << 23) | frac;
    
}

int main() {
  float ftest = 2e-40;
  float_bits test = ftest;
  float fresult = ftest / 2;
  float_bits result = float_half(0x00022d85); /* 500 */
  show_float(fresult); /* 250 */
  show_int(result); /* 250 ? */
  
  return 0;
}
