#include "csapp.h"

extern pid_t fg_job;
extern sigjmp_buf buf;

void sigint_handler(int sig) {
  if (fg_job != 0) {
    siglongjmp(buf, 1);
  }
  // should reap children first
  _exit(0);
}
