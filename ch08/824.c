/* $begin waitpid1 */
#include "csapp.h"
#include <signal.h>
#include <stdio.h>
#define N 2

int main() 
{
    int status, i;
    pid_t pid;
    char *s = "hello world";

    /* Parent creates N children */
    for (i = 0; i < N; i++)                        //line:ecf:waitpid1:for
      if ((pid = Fork()) == 0) {  /* Child */      //line:ecf:waitpid1:fork
        s[0] = 'H';
        exit(100+i); 
      }  
                                                   //line:ecf:waitpid1:exit

    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0) { //line:ecf:waitpid1:waitpid
      if (WIFEXITED(status))                    //line:ecf:waitpid1:wifexited
        printf("child %d terminated normally with exit status=%d\n",
               pid, WEXITSTATUS(status));     //line:ecf:waitpid1:wexitstatus
      else if (WIFSIGNALED(status)) {
        char message[50];
        sprintf(message, "child %d terminated by Signal %d", pid, WTERMSIG(status));
        psignal(WTERMSIG(status), message);
      } else
        printf("child %d terminated abnormally\n", pid);
    }

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)                          //line:ecf:waitpid1:errno
	unix_error("waitpid error");

    exit(0);
}
/* $end waitpid1 */
