#include <stdio.h>
#include <assert.h>
#include <limits.h>

typedef enum{NEG, ZERO, POS, OTHER} range_t;

range_t find_range_alt374(float x);

range_t find_range(float x) {
  int result;
  if (x<0)
    result = NEG;
  else if (x == 0)
    result = ZERO;
  else if (x > 0)
    result = POS;
  else
    result = OTHER;

  return result;
}

int main() {
  int val;
  float *fp;

  for (val = INT_MIN; val< INT_MAX; val++) {
    fp = &val;
    assert(find_range(*fp) == find_range_alt374(*fp));
  }

  return 0;
}


