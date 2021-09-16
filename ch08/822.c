#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "csapp.h"

#define MAXARG 128
#define MAXCLINE 512
#define MAXCOMMAND 512

int mysystem(char *command);
void parse(char *command, char *argv[], char text[]);
void parse_argv(int argc, char *argv[], char *command);

int main(int argc, char *argv[]) {
  int status;
  char command[MAXCOMMAND];

  if (argc < 2) {
    printf("Usage: ./mysystem command\n");
    return 1;
  }
  
  parse_argv(argc, argv, command);
  status = mysystem(command);
  printf("%d\n", status);

  return 0;
}

void parse_argv(int argc, char *argv[], char *command) {
  char *command_start = command;
  for (int i=1; i < argc; i++) {
    for (int j=0; argv[i][j] != '\0'; j++)
      if (command - command_start < MAXCOMMAND - 1)
        *command++ = argv[i][j];
    *command++ = ' ';
  }
  *--command = '\0';
}

int mysystem(char *command) {
  char *args[MAXARG];
  char text[MAXLINE];
  parse(command, args, text);

  pid_t pid;
  if ((pid = Fork()) == 0) {
  /* child */
    execve("/bin/sh", args, environ);
  }

  int status;
  Waitpid(-1, &status, 0);

  if (WIFEXITED(status))
    return WEXITSTATUS(status);
    
  return -1;
}

void parse(char *command, char *args[], char text[]) {
  int i = 0;
  char *start_text = text;
  args[i++] = text;
  *text++ = '/';
  *text++ = 'b';
  *text++ = 'i';
  *text++ = 'n';
  *text++ = '/';
  *text++ = 's';
  *text++ = 'h';
  *text++ = '\0';
  args[i++] = text;
  *text++ = '-';
  *text++ = 'c';
  *text++ = '\0';

  while (*command++ == ' ')
    ;
  command--;
  args[i++] = text;
        
  while (*command != '\0') {
    if (text - start_text >= MAXCLINE - 1)
      unix_error("Command too long\n");
    *text++ = *command++;      
  }
  *text++='\0';


  args[i] = NULL;
}


