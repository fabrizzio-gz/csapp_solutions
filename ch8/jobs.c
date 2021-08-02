#include "csapp.h"

extern pid_t jobs[];

void save_job(pid_t pid, int n) {
  for (int i=0; i < n; i++)
    if (jobs[i] == 0) {
      jobs[i] = pid;
      return;
    }

  fprintf(stderr, "Reached job limit: %d. Can't save job", n);
  // reap children and quit
  // reap_children();
  // exit(0);
  return;
}

int get_jid(pid_t pid, int n) {
  for (int i=0; i < n; i++)
    if (jobs[i] == pid)
      // JID is jobs array index + 1
      return i+1;

  // reap_children();
  char s[64];
  sprintf(s, "get_jid: Unkown PID %d", pid);
  unix_error(s);
}

void release_job(pid_t pid, int n) {
  for (int i=0; i < n; i++)
    if (jobs[i] == pid) {
      jobs[i] = 0;
      return;
    }

  // reap_children();
  char s[64];
  sprintf(s, "release_job: Unkown PID %d", pid);
  unix_error(s);
}

void print_jobs(int n) {
  for (int i=0; i < n; i++)
    if (jobs[i] != 0)
      printf("[%d] %d\tTODO:STATUS\tTODO:args\n", i+1, jobs[i]);
}
