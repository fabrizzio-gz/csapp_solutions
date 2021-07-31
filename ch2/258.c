#include <stdio.h>
#include <limits.h>


int is_little_endian(void);

int main() {

  if (is_little_endian())
    printf("Computer is little endian!\n");
  else
    printf("Compputer is big endian!\n");
  
  return 0;
}


int is_little_endian() {
  int val = INT_MIN;  /* 1000 0000 0000 .... */
  unsigned char *p = &val;
  printf("Last byte is: %.2x\n", p[sizeof(int) - 1]);
  return p[0] == 0x0;
}
