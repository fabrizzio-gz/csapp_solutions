#include <assert.h>

void bubble_p(long *data, long count);

int main() {
  long data[4] = {4, 3, 2 ,1};
  bubble_p(data, 4);
  assert(data[0] == 1);
  assert(data[1] == 2);
  assert(data[2] == 3);
  assert(data[3] == 4);
  return 0;
}

/* Bubble sort: Array version */
/* void bubble_a(long *data, long count) { */
/*   long i, last; */
/*   for (last = count-1; last > 0; last--) { */
/*     for (i=0; i < last; i++) */
/*       if (data[i+1] < data[i]) { */
/*         /\* Swap adjacent elements *\/ */
/*         long t = data[i+1]; */
/*         data[i+1] = data[i]; */
/*         data[i] = t; */
/*       } */
/*   } */
/* } */

/* A */
void bubble_p(long *data, long count) {
  long *p, *last;
  for (last = data+count-1; last > data; last--) {
    for (p=data; p < last; p++)
      if(*(p+1) < *p) {
        long t = *(p+1);
        *(p+1) = *p;
        *p = t;
      }
  }
}
