#include "csapp.h"

extern pid_t fg_job;
extern sigjmp_buf buf;
extern volatile sig_atomic_t terminate;
extern volatile sig_atomic_t stop;

void sigint_handler(int sig) {
  if (fg_job != 0) {
    if (sig == 2)        /* SIGINT */
      terminate = 1;
    else if (sig == 20)  /* SIGTSTP */
      stop = 1;
    siglongjmp(buf, 1);
  }

  return;
}
