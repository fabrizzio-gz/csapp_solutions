#include "csapp.h"

#define MAX_THREAD 1024

void *thread(void *vargp);                    



int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./main <num_threads>\n");
    exit(0);
  }

  int n = atoi(argv[1]);

  if ((!n) || (n > MAX_THREAD)) {
    fprintf(stderr, "Invalid number of threads: %s\n", argv[1]);
    exit(0);
  }
  
  pthread_t tid[n];

  for (int i = 0; i < n; i++)
    Pthread_create(&tid[i], NULL, thread, NULL);

  for (int i = 0; i < n; i++)
    Pthread_join(tid[i], NULL);                  

  exit(0);
}

void *thread(void *vargp) 
{
  printf("Hello, world!\n");                 
  return NULL;                              
}                                            
