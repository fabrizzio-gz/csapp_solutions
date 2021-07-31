#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define BIAS 127
#define K 23

typedef unsigned float_bits;

/* Convert 32-bits int to float */
float_bits float_i2f(int i) {
  /* Special case : 0 is not a normalized value */
  if (i==0)
    return 0;
  /* sign bit */
  unsigned s = i>>31;
  /* Exponent */
  unsigned E;
  if (i != INT_MIN) 
    E = (int) (log(i<0 ? -i : i)/log(2));
  else
    /* INT_MIN = -1.0 * 2^31 */
    E = 31; 
  unsigned exp = E + BIAS;
  /* Magnificand*/
  unsigned M;
  if (i != INT_MIN)
    M = i>0 ? i : -i;
  else
    /* INT_MIN = -1.0 x 2^31*/
    M = 1<<31;
  unsigned frac = M ^ (1<<E);
      
  /* Displace frac to start at bit position K=23 */
  if (E>K) {
    /* Frac field too long */
    /* Round to closest  */
    unsigned round = frac >> (E - K - 1) & 1;
    /* Round-to-even modification */
    unsigned last_frac_bit = frac >> (E - K) & 1;
    unsigned rounded_off_digits = frac << 32 - E + K >> 32 - E + K;
    unsigned one_followed_by_zeros = 1 << E - K - 1;
    if (rounded_off_digits == one_followed_by_zeros)
      round = last_frac_bit;

    /* Truncate to first 23th digits */
    frac >>= E-K;

    /* Special case: frac = 11..1 */
    unsigned all_ones = -1 & 0x7FFFFF;
    if (frac == all_ones && round) {
      frac = 0;
      exp++;
      round = 0;
    }
    /* Round when necessary */
    frac += round;
  } else
    /* Too short: Pad to the right with zeros */
    frac <<= K-E;
  
  return s<<31 | exp<<K | frac;
}

int main() {
  int i;
  float *fp; 
  float_bits f;
  
  for (i=INT_MIN; i<INT_MAX; i++) {
    f = float_i2f(i);
    fp = &f;
    if (*fp != (float) i)
      printf("Casting not equal for value: %d\nConverted value is %.0f\nCasted value is %.0f\n", i, *fp, (float) i);
  }
}
