#include "csapp.h"

char *tfgets(char *s, int size, FILE *stream);

int main()  {
  char buf[MAXLINE];

  if (tfgets(buf, MAXLINE, stdin) == NULL)
    printf("BOOM!\n");
  else 
    printf("%s", buf);
  
  exit(0);
}

static sigjmp_buf buf;

static void handler(int sig) {
  siglongjmp(buf, 1);
}

char *tfgets(char *s, int size, FILE *stream) {
  pid_t pid;
  int status;

  Signal(SIGUSR1, handler);

  if (sigsetjmp(buf, 1)) {
    Wait(&status);
    return NULL;
  }
  
  if ((pid = Fork()) == 0) {
    /* Child process used as a timer */
    sleep(5);
    Kill(getppid(), SIGUSR1);
    exit(0);
  }
  s = Fgets(s, size, stream);
  Kill(pid, SIGTERM);
  Wait(&status);
  return s;
}
