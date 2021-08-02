#include "csapp.h"

#ifndef MAXJOBS
#define MAXJOBS 16
#endif

extern pid_t jobs[];
static char job_status[MAXJOBS] = {0}; /* 0: Running, 1: Stopped */
static char *status[2] = {"Running", "Stopped"};

void save_job(pid_t pid, int n) {
  for (int i=0; i < MAXJOBS; i++)
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

int get_jid(pid_t pid) {
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] == pid)
      // JID is jobs array index + 1
      return i+1;

  // reap_children();
  char s[64];
  sprintf(s, "get_jid: Unkown PID %d", pid);
  unix_error(s);
}

void release_job(pid_t pid) {
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] == pid) {
      jobs[i] = 0;
      return;
    }

  // reap_children();
  char s[64];
  sprintf(s, "release_job: Unkown PID %d", pid);
  unix_error(s);
}

void print_jobs() {
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] != 0)
      printf("[%d] %d %s\tTODO:args\n", i+1, jobs[i], status[job_status[i]]);
}
