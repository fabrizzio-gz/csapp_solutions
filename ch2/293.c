#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "show-bytes.c"


typedef unsigned float_bits;

int is_nan(float_bits f) {
  unsigned exp = f>> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;

  return exp == 0xFF && frac;
}

float_bits float_absval(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFFF;

  show_int(f);
  sign = 0;
  if (is_nan(f))
    return f;

  unsigned test = (sign << 31) | (exp << 23) | frac;
  show_int(test);
  return (sign << 31) | (exp << 23) | frac;
}

void test() {
  unsigned val;
  float fval;
  float_bits absval;

  for (val=1; val != 0; val++) {
    fval = (float) val;
    absval = float_absval(val);
    if (fval < 0) 
      printf("%f %f\n", fval, (float) absval);
    if (is_nan(fval))
      assert(is_nan(absval));
    else
      assert(abs(fval) == absval);
  }
}

int main() {
  /* tests do not work because of casting */
  test();
  float_bits test =  float_absval(0xc0200000); /* -2.5 */
  printf("%f\n", (float) test);

  return 0;
}
