#include "csapp.h"

#ifndef MAXJOBS
#define MAXJOBS  16
#endif
#define MAXCMD   16  

extern pid_t jobs[];

/* local globals */
static char job_status[MAXJOBS] = {0}; /* 0: Running, 1: Stopped */
static char *status[2] = {"Running", "Stopped"};
/* Save only MAXCMD chars of job command */
static char job_cmd[MAXJOBS][MAXCMD];

void save_job(pid_t pid) {
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] == 0) {
      jobs[i] = pid;
      return;
    }

  fprintf(stderr, "Reached job limit: %d. Can't save job", MAXJOBS);
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
  jobs[get_jid(pid) - 1] = 0;
  return;
}

void print_jobs() {
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] != 0)
      printf("[%d] %d %s\t%s\n", i+1, jobs[i], status[job_status[i]], job_cmd[i]);
}

void save_job_cmd(pid_t pid, char *argv[]) {
  int job_i = get_jid(pid) - 1;

  int i = 0;
  int arg = 0;
  int j = 0;
  while (i < MAXCMD - 1 && argv[arg] != NULL) {
    job_cmd[job_i][i++] = argv[arg][j++];
    if (argv[arg][j] == '\0') {
      arg++;
      j++;
    }
  }
}
