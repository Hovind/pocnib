#include "matrix.h"

#define N 3
#define M 3

int main(void) {
  matrix * a = new(N, N);
  matrix * b = new(N, M);
  matrix * x = new(N, 1);
  matrix * u = new(N, 1);
  fill((double[N*N]){1, 2, 3, 4, 5, 6, 7, 8, 9}, N*N, a);
  fill((double[N*M]){1, 2, 3, 4, 5, 6}, N*M, b);
  while (1) {
    
  }
}


