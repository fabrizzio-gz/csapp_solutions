/*
 * Calculate c = a. b matrix.
 * Sequential version (single thread).
 */
  
#include <stdio.h>

#define M 40
#define N 40
#define P 40

void matrix_mul(int a[M][N], int b[N][P], int c[M][P]);
void init(int rows, int cols, int mat[rows][cols]);

int main(int argc, char *argv[]) {
  int a[M][N];
  int b[N][P];
  int c[M][P] = {0};

  /* Fill matrices with ones */
  init(M, N, a);
  init(N, P, b);

  matrix_mul(a, b ,c);

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
 * Matrix a dimensions (m rows, n cols)
 * Matrix b dimensions (n rrows, p cols)
 * Calculates matrix c = a x b (m rows, p cols)
*/
void matrix_mul(int a[M][N], int b[N][P], int c[M][P]) {
  for (int i = 0; i < M; i++)
    for (int j = 0; j < P ; j++)
      for (int k = 0; k < N; k++) 
        c[i][j] += a[i][k] * b[k][j];
}     
       
