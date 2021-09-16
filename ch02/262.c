#include <stdio.h>

int int_shifts_are_arithmetic();

int main() {
  if (int_shifts_are_arithmetic())
    printf("Arithmetic right shift!\n");
  else
    printf("Logic right shift!\n");

  return 0;
}

int int_shifts_are_arithmetic() {
  int val = -1; /* 111.. 1 */
  return (val >> (sizeof(int) * 8 - 1)) == -1;
  /* Alternative: negative val shifted right stays negative */
  return -2 >> 1 < 0;
}
