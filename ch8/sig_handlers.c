#include "csapp.h"

extern pid_t fg_job;
extern sigjmp_buf buf;
extern volatile sig_atomic_t terminate;

void sigint_handler(int sig) {
  if (fg_job != 0) {
    if (sig == 2)
      terminate = 1;
    siglongjmp(buf, 1);
  }
  // should reap children first
  _exit(0);
}


