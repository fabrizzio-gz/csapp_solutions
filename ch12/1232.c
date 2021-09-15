#include "csapp.h"

char *tfgets(char *s, int size, FILE *stream);

int main() {
  char buf[MAXLINE];
  
  if (tfgets(buf, MAXLINE, stdin) == NULL)
    printf("BOOM!\n");
  else 
    printf("%s", buf);
  
  exit(0);
}

char *tfgets(char *s, int size, FILE *stream) {
  fd_set ready_set;
  struct timeval timeout;

  FD_ZERO(&ready_set);
  FD_SET(STDIN_FILENO, &ready_set);
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  Select(STDIN_FILENO + 1, &ready_set, NULL, NULL, &timeout);
  if (FD_ISSET(STDIN_FILENO, &ready_set))
    return fgets(s, size, stream);

  return NULL;
}

