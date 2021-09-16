#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main() {
  int fd;
  /* File needs to be previously created */
  /* $ echo "Hello, world!" >> hello.txt */
  if ((fd = open("hello.txt", O_RDWR)) == -1) {
    printf("Couldn't open file\n");
    return 1;
  }

  char *ptr;
  if ((ptr = mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void *) -1) {
    printf("mmap error\n");
    return 1;
  }
  *ptr = 'J'; 

  return 0;
}
