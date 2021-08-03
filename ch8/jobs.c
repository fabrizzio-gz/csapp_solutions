#include "csapp.h"

#ifndef MAXJOBS
#define MAXJOBS  16
#endif
#define MAXCMD   16

static void send_sig(pid_t pid, int sig);
static pid_t parse_pid(char *arg);
static void print_finished_job(pid_t pid);

extern pid_t jobs[];
extern pid_t fg_job;

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
  return -1;
}

void release_job(pid_t pid) {
  jobs[get_jid(pid) - 1] = 0;
}

void print_jobs() {
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] != 0)
      printf("[%d] %d %s\t%s\n", i+1, jobs[i], status[(int) job_status[i]], job_cmd[i]);
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

void resume_fg_job(char **argv) {
  pid_t pid;
  if ((pid = parse_pid(argv[1])) > 0) {
    fg_job = pid;
    send_sig(pid, 18);
    if (waitpid(pid, NULL, 0) < 0)
      unix_error("waitfg: waitpid error");
    return;
  }

  printf("%s: No such %s\n", argv[1], *argv[1] == '%' ? "job" : "process");
}

void resume_bg_job(char **argv) {
  pid_t pid;
  if ((pid = parse_pid(argv[1])) > 0) {
    send_sig(pid, 18);
    int jid = get_jid(pid);
    job_status[jid-1] = 0; /* running */
    printf("[%d] %d\t%s\n", jid, pid, job_cmd[jid-1]);
    return;
  }
  
  printf("%s: No such %s\n", argv[1], *argv[1] == '%' ? "job" : "process");
}

void terminate_fg() {
  send_sig(fg_job, 2);

  int status;
  Waitpid(fg_job, &status, 0);
  if (WIFSIGNALED(status)) {
    char s[50];
    sprintf(s, "Job %d terminated by signal", fg_job);
    psignal(WTERMSIG(status), s);
  }
    
  release_job(fg_job);
  fg_job = 0;
}

void stop_fg() {
  send_sig(fg_job, 20);

  int status;
  Waitpid(fg_job, &status, WUNTRACED);
  if (WIFSTOPPED(status)) {
    char s[50];
    sprintf(s, "Job [%d] %d stopped by signal", get_jid(fg_job), fg_job);
    psignal(WSTOPSIG(status), s);
  }

  /* Set job status to stopped */
  job_status[get_jid(fg_job) -1] = 1;
  fg_job = 0;
}

void reap_finished_children() {
  pid_t finished_pid;
  while ((finished_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    print_finished_job(finished_pid);
    release_job(finished_pid);
  }
}

static void send_sig(pid_t pid, int sig) {
  pid_t pgid = -pid;
  /* kill process group, if not set, kill job 
   * (due to race conditions) */
  if (kill(pgid, sig) < 0) {
    Kill(pid, sig);
  }
}

static pid_t parse_pid(char *arg) {
  int jid;
  if (*arg == '%') {
    jid = atoi((arg+1));
    if (jid >= MAXJOBS)
      return -1;
    /* check jid is set to a valid process */ 
    return jobs[jid -1] > 0 ? jobs[jid-1] : -1;
  }

  int pid;
  pid = atoi(arg);
  if (pid <= 0)
    return -1;
  for (int i=0; i < MAXJOBS; i++)
    if (jobs[i] == pid)
      return pid;
  /* pid is not part of jobs */
  return -1;
}

static void print_finished_job(pid_t pid) {
  int job_i = get_jid(pid) - 1;
  printf("[%d] %d Done\t%s\n", job_i + 1, pid, job_cmd[job_i]);
}
