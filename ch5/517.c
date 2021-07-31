#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void *basic_memset(void *s, int c, size_t n);
void *my_memset(void *s, int c, size_t n);

int main() {
  int size = 1000000;
  int loops = 10000;
  int i;
  clock_t start, end;
  double cpu_time_used;
  char *test1 = (char *) malloc(size);
  char *test2 = (char *) malloc(size);

  //if (!test1 || !test2) {
  //printf("Couldn't allocate memory\n");
  //  return 1;
  //};

  char val = 0x56;
  
  //printf("Starting basic memset...");
  // start = clock();
  for (i=0; i < loops; i++)
   basic_memset((void *)test1, val, size);
    //end = clock();
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    //printf("It took %f seconds.\n", cpu_time_used);
  
  /* printf("Starting new memset..."); */
  /* start = clock(); */
  //for (i=0; i < loops; i++)
  //  my_memset((void *)test2, val, size); 
  /* end = clock(); */
  /* cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; */
  /* printf("It took %f seconds.\n", cpu_time_used); */

  /* for (int i = 0; i < size; i++) { */
  /*   assert(test1[i] == val); */
  /*   assert(test2[i] == val); */
  /*   // printf("%c\n", test2[i]); */
  /* } */

  return 0;
}

void *basic_memset(void *s, int c, size_t n) {
  size_t cnt = 0;
  unsigned char *schar = s;
  while (cnt < n) {
    *schar++ = (unsigned char) c;
    cnt++;
  }

  return s;
}

void *my_memset(void *s, int c, size_t n) {
  size_t cnt = 0;
  int k = sizeof(unsigned long);
  unsigned char *schar = s;
  unsigned char first_byte = (unsigned char) c;
  /* mask is 2 last bits of addr or 3 last bits
   * depending on value of k (either 4 or 8) 
   */
  int mask = k == 4 ? 0x3 : 0x7; 
  
  while (!((long) schar & mask) && cnt < n) {
    *schar++ = first_byte;
    cnt++;
  }

  unsigned long *slong = schar;
  unsigned long eight_bytes = 0;
  for (int i = 0; i < k; i++)
    eight_bytes |= (unsigned long) first_byte << (8*i);

  // printf("8b: %lx\n", eight_bytes);
  while (cnt < n - k + 1) {
    *slong++ = eight_bytes;
    cnt += k;
  }

  schar = slong;
  while (cnt < n) {
    *schar++ = first_byte;
    cnt++;
  }

  return s;
}
