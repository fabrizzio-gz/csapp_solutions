/* $begin shellmain */
#include "csapp.h"
#include "jobs.h"
#include "sig_handlers.h"
#define MAXARGS   128
#ifndef MAXJOBS
#define MAXJOBS   16
#endif

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
void reap_finished_children();

/* globals */
pid_t jobs[MAXJOBS] = {0};
pid_t fg_job = 0;
sigjmp_buf buf;
volatile sig_atomic_t terminate = 0;
volatile sig_atomic_t stop = 0;

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

    Signal(2, sigint_handler);   /* SIGINT */
    Signal(20, sigint_handler);  /* SIGTSTP */
    if (sigsetjmp(buf, 1) > 0 ) {
      if (terminate == 1) {
        terminate = 0;
        terminate_fg();
      } else if (stop == 1) {
        stop = 0;
        stop_fg();
      }
    }
        
    while (1) {
	/* Read */
	printf("> ");                   
	Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);

	/* Evaluate */
	eval(cmdline);
        reap_finished_children();
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
        if ((pid = Fork()) == 0) {   /* Child runs user job */
          /* Set pgid to children pid */
          Setpgid(0, 0);
          if (execve(argv[0], argv, environ) < 0) {
            printf("%s: Command not found.\n", argv[0]);
            exit(0);
          }
        }
        
        /* Parent stores child PID */
        save_job(pid);
        
	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
            fg_job = pid;
	    if (waitpid(pid, &status, 0) < 0)
		unix_error("waitfg: waitpid error");
            release_job(pid);
            fg_job = 0;
	}
	else {
          save_job_cmd(pid, argv);
          printf("[%d] %d %s", get_jid(pid), pid, cmdline);
        } 
	    
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
	exit(0);
    if(!strcmp(argv[0], "jobs")) {
      print_jobs();
      return 1;
    }
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */

void reap_finished_children() {
  pid_t finished_pid;
  while ((finished_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    print_finished_job(finished_pid);
    release_job(finished_pid);
  }
}
