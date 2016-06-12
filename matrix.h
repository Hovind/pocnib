#include "stdint.h"

typedef struct matrix_struct {
  uint8_t m, n;
  double * data;
} matrix;

int8_t eye(matrix * mat);
int8_t diag(double * diagonal, uint8_t elements, matrix * mat);
int8_t fill(double * content, uint8_t elements, matrix * mat);
matrix * new(uint8_t m, uint8_t n);
int8_t product(matrix * lhs, matrix * rhs, matrix * result);
int8_t transpose(matrix * o, matrix * t);
int8_t solve(matrix * a, matrix * x, matrix * b);
