/* $begin show-bytes */
#include <stdio.h>
/* $end show-bytes */
#include <stdlib.h>
#include <string.h>
/* $begin show-bytes */

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; i++)
	printf(" %.2x", start[i]);    //line:data:show_bytes_printf
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int)); //line:data:show_bytes_amp1
}

void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long)); //line:data:show_bytes_amp1
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float)); //line:data:show_bytes_amp2
}

void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double)); //line:data:show_bytes_amp2
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *)); //line:data:show_bytes_amp3
}
/* $end show-bytes */


void simple_show_a() {
/* $begin simple-show-a */
long int val = 0x87654321;
byte_pointer valp = (byte_pointer) &val;
show_bytes(valp, 1); /* A. */
show_bytes(valp, 2); /* B. */
show_bytes(valp, 3); /* C. */
 printf("Showing all bits:\n");
 show_long(val);
/* $end simple-show-a */
}

void double_eg() {
  int x = 3.14;
  float f = 3.15;
  printf("For x = %d\n", x);
  show_int(x);
  show_float(f);

  x = 3510593;
  f = (float) x;
  printf("For x = %d\n", x);
  show_int(x);
  show_float(f);

}

int main(int argc, char *argv[])
{
    int val = 12345;

    if (argc > 1) {
	if (argc > 1) {
	    val = strtol(argv[1], NULL, 0);
	}
    } else {
	printf("Calling simple_show_a\n");
	simple_show_a();
	printf("Calling double_eg\n");
	double_eg();
    }
    return 0;
}
