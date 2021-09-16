#include <unistd.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[], char *envp[]) {
  execve("/bin/ls", argv, envp);
  fprintf(stderr, "Error: %s\n", strerror(errno));
  return 0;
}
