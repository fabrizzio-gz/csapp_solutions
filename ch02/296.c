#include <limits.h>
#include <assert.h>

#define NAN 0x80000000
#define BIAS 127
#define K 8
#define N 23

typedef unsigned float_bits;

/* 
 * Compute (int) f.
 * If conversion causes overflow or f is NaN, return 0x80000000
 */
int float_f2i(float_bits f) {
  unsigned s = f >> (K + N);
  unsigned exp = f >> N & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  
  /* Denormalized values round to 0 */
  if (exp == 0)
    return 0;
  /* f is NaN */
  if (exp == 0xFF)
    return NAN;
  /* Normalized values */
  int x;
  int E = exp - BIAS;
  /* Normalized value less than 0, return 0 */
  if (E < 0)
    return 0;
  /* Overflow condition */
  if (E > 30)
    return NAN;
  x = 1 << E;
  if (E < N)
    x |= frac >> (N - E);
  else
    x |= frac << (E - N);

  /* Negative values */
  if (s == 1)
    x = ~x + 1;

  return x;  
}

int main() {
  int bits;
  float *fp;
  float_bits *fbp;
  
  for (bits = INT_MIN; bits < INT_MAX; bits++) {
    fp = &bits;
    fbp = &bits;
    assert((int) *fp == float_f2i(*fbp));
  }

  return 0;
}
