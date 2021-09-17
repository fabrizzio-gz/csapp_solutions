#include "csapp.h"

char *tfgets(char *s, int size, FILE *stream);

int main() {
  char buf[MAXLINE];

  if (tfgets(buf, MAXLINE, stdin) == NULL)
    printf("BOOM!\n");
  else 
    printf("%s", buf);

  exit(0);
}

struct t_args {
  char *s;
  int size;
  FILE *stream;
  char *retp; /* fgets return pointer */
};

sem_t mutex;

void *gets_thread(void *vargp);
void *timeout(void *vargp);

char *tfgets(char *s, int size, FILE *stream) {
  pthread_t tid;
  struct t_args args;
  
  args.s = s;
  args.size = size;
  args.stream = stream;
  args.retp = NULL;
  
  Sem_init(&mutex, 0, 0);
  
  Pthread_create(&tid, NULL, gets_thread, (void *)&args);
  P(&mutex);
  return args.retp;
}

void *gets_thread(void *vargp) {
  struct t_args *argp = (struct t_args *)vargp;
  pthread_t tid, tid_self = pthread_self();
  
  Pthread_create(&tid, NULL, timeout, (void *)&tid_self);
  argp->retp = Fgets(argp->s, argp->size, argp->stream);
  Pthread_cancel(tid);
  Pthread_join(tid, NULL);
  V(&mutex);
  return NULL;
}

void *timeout(void *vargp) {
  pthread_t peer_tid = *(pthread_t *)vargp;
  
  Sleep(5);
  Pthread_cancel(peer_tid);
  Pthread_join(peer_tid, NULL);
  V(&mutex);
  return NULL;
}
