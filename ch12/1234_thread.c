/*
 * Calculate c = a. b matrix.
 * Threaded version (four threads).
 */
#include "csapp.h"

#define M 40
#define N 40
#define P 40
#define THREADS 4

void *matrix_mul(void *vargp);
void init(int rows, int cols, int mat[rows][cols]);

/* Matrices defined as globals */
int a[M][N];
int b[N][P];
int c[M][P] = {0};

int main(int argc, char *argv[]) {
  pthread_t tid[THREADS];
  int id[THREADS];

  /* Fill matrices with ones */
  init(M, N, a);
  init(N, P, b);

  for (int i = 0; i < THREADS; i++) {
    id[i] = i;
    Pthread_create(&tid[i], NULL, matrix_mul, (void *)&id[i]);
  }

    
  for (int i = 0; i < THREADS; i++)
    Pthread_join(tid[i], NULL);
  

  /* Print resultant matrix */
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < P; j++)
      printf("%d ", c[i][j]);
    printf("\n");
  }

  return 0;
}

void init(int rows, int cols, int mat[rows][cols]) {
  for (int i=0; i < rows; i++)
    for (int j=0; j < cols; j++)
      mat[i][j] = 1;
}

/*
 * Threaded matrix multiplication
 * Matrix a dimensions (m rows, n cols)
 * Matrix b dimensions (n rows, p cols)
 * Calculates matrix c = a x b (m rows, p cols)
 * Divide matrix b in P/THREADS cols
 * Each thread calculates matrix multiplication only along
 * those columns. 
*/
void *matrix_mul(void *vargp) {
  int id = *((int *)vargp); 
  for (int i = 0; i < M; i++)
    for (int j = id * (P/4); j < (id + 1)* (P/4) ; j++)
      for (int k = 0; k < N; k++) 
        c[i][j] += a[i][k] * b[k][j];
}
