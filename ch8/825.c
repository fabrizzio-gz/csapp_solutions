#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "csapp.h"

jmp_buf buf;

void handler(int sig) {
  siglongjmp(buf, 1);
}

char *tfgets(char *s, int size, FILE *stream) {
  char *s_ret;
  if (!sigsetjmp(buf, 1)) 
    Signal(14, handler);
  else
    return NULL;
  alarm(5);
  s_ret = fgets(s, size, stream);
  return s_ret;
}

int main() {
  char *s;
  s = (char *) malloc(100 * sizeof(char));
  s = tfgets(s, 100, stdin);
  if (s)
    printf("Obtained %s", s);
  else
    printf("Timed out! Returned %s\n", s);
  
  return 0;
}

