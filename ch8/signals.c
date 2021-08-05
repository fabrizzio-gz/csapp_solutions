#include "csapp.h"

extern pid_t fg_job;
extern sigjmp_buf buf;
extern volatile sig_atomic_t terminate;
extern volatile sig_atomic_t stop;

void sigint_handler(int sig) {
  if (fg_job != 0) {
    if (sig == SIGINT)   
      terminate = 1;
    else if (sig == SIGTSTP)  
      stop = 1;
    siglongjmp(buf, 1);
  }

  Sio_puts("\n");
  return;
}

handler_t *add_signal_handler(int signum, handler_t *handler, int blocked_signum) {
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    sigaddset(&action.sa_mask, blocked_signum);
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}
