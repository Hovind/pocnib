#include "stdlib.h"
#include "stdio.h"
#include "matrix.h"

int8_t is_index_valid(uint8_t r, uint8_t c, matrix * mat) {
  return r < mat->m && c < mat->n;
}
int8_t set(uint8_t r, uint8_t c, double value, matrix * mat) {
  if (!is_index_valid(r, c, mat)) {
    return 1;
  }
  mat->data[c*mat->m+r] = value;
  return 0;
}
double get(uint8_t r, uint8_t c, matrix * mat) {
  if (is_index_valid(r, c, mat)) {
    return mat->data[c*mat->m+r];
  }
  return 0.0;
}
int8_t eye(matrix * mat) {
  if (mat->m != mat->n) {
    return 1;
  }
  for (uint8_t i = 0; i < mat->m; ++i) {
    set(i, i, 1.0f, mat);
  }
  return 0;
}
int8_t diag(double * diagonal, uint8_t elements, matrix * mat) {
  if (elements != mat->m || elements != mat->n) {
    return 1;
  }
  for (uint8_t i = 0; i < elements; ++i) {
    set(i, i, diagonal[i], mat);
  }
  return 0;
}
int8_t fill(double * content, uint8_t elements, matrix * mat) {
  if (elements != mat->m * mat->n) {
    return 1;
  }
  for (uint8_t i = 0; i < elements; ++i) {
    set(i / mat->n, i % mat->n, content[i], mat);
  }
  return 0;
}
matrix * new(uint8_t m, uint8_t n) {
  matrix * mat;
  mat = malloc(sizeof(*mat));
  mat->m = m;
  mat->n = n;
  size_t element_size = sizeof(*(mat->data));
  size_t elements = m * n;
  mat->data = calloc(elements, element_size);
  return mat;
}
int8_t print(matrix * mat) {
  for (uint8_t r = 0; r < mat->m; ++r) {
    for (uint8_t c = 0; c < mat->n; ++c) {
      printf("%lf ", get(r, c, mat));
    }
    printf("\n");
  }
  return 0;
}
int8_t product(matrix * lhs, matrix * rhs, matrix * result) {
  if (lhs->n != rhs->m || lhs->m != result->m || rhs->n != result->n) {
    return 1;
  }
  for (uint8_t r = 0; r < result->m; ++r) {
    for (uint8_t c = 0; c < result->n; ++c) {
      double sum = 0;
      for (uint8_t i = 0; i < lhs->n; ++i) {
        sum += get(r, i, lhs) * get(i, c, rhs);
      }
      set(r, c, sum, result);
    }
  }
  return 0;
}
int8_t transpose(matrix * o, matrix * t) {
  //checks
  for (uint8_t i = 0; i < o->m; ++i) {
    for (uint8_t j = 0; j < o->n; ++j) {
      set(i, j, get(j, i, o), t);
    }
  }
  return 0;
}
int8_t crout(matrix * a, matrix * l, matrix * u) {
  if (a->m != a->n || l->m != l->n || u->m != u->n || a->m != l->m || l->m != u->m) {
    return 1;
  }
  for (uint8_t i = 0; i < a->m; i++) {
    set(i, i, 1.0f, u);
  }
  for (uint8_t j = 0; j < a->m; ++j) {
    for (uint8_t i = j; i < a->m; ++i) {
      double sum = 0;
      for (uint8_t k = 0; k < j; ++k) {
        sum += get(i, k, l) * get(k, j, u);;
      }
      set(i, j, get(i, j, a) - sum, l);
    }

    for (uint8_t i = j; i < a->m; i++) {
      double sum = 0;
      for(uint8_t k = 0; k < j; k++) {
        sum = sum + get(j, k, l) * get(k, i, u);
      }
      if (get(j, j, l) == 0) {
        return 2;
      }
      set(j, i, (get(j, i, a) - sum) / get(j, j, l), u);
    }
  }
  return 0;
}
//L*y=b U*x=y L*U*x=b A*x=b
int8_t solve_upper(matrix * u, matrix * x, matrix * y) {
  //checks
  for (uint8_t i = u->m - 1; i < u->m; --i) {
    double sum = 0;
    for (uint8_t j = i + 1; j < u->m; ++j) {
      sum += get(i, j, u) * get(j, 0, x);
    }
    set(i, 0, (get(i, 0, y) - sum) / get(i, i, u), x);
  }
  return 0;
}
int8_t solve_lower(matrix * l, matrix * y, matrix * b) {
  //checks
  for (uint8_t i = 0; i < l->m; ++i) {
    double sum = 0;
    for (uint8_t j = 0; j < i; ++j) {
      sum += get(i, j, l) * get(j, 0, y);
    }
    set(i, 0, (get(i, 0, b) - sum) / get(i, i, l), y);
  }
  return 0;
}
int8_t solve(matrix * a, matrix * x, matrix * b) {
  //checks
  matrix * l = new(a->m, a->n);
  matrix * u = new(a->m, a->n);  
  matrix * y = new(a->m, 1);
  crout(a, l, u);
  solve_lower(l, y, b);
  solve_upper(u, x, y);
  return 0;
}
int8_t test_crout() {
  matrix * l = new(2, 2);
  matrix * u = new(2, 2);
  matrix * e = new(2, 2);
  matrix * f = new(2, 2);
  fill((double[4]){4, 3, 6, 3}, 4, e);
  crout(e, l, u);
  product(l, u, f);
  print(e);
  print(l);
  print(u);
  print(f);
  return 0;
}
int8_t test_product() {
  matrix * a = new(3, 3);
  matrix * b = new(3, 1);
  matrix * c = new(3, 1);
  eye(a);
  set(1, 3, 5, a);
  set(1, 2, 5, a);
  fill((double[3]){3, 4, 3}, 3, b);
  product(a, b, c);
  print(a);
  print(b);
  print(c);
  return 0;
}
int8_t test_diag() {
  matrix * d = new(5, 5);
  diag((double[5]){1, 2, 3, 4, 5}, 5, d);
  print(d);
  return 0;
}
int8_t test_solvers() {
  matrix * a = new(2, 2);
  matrix * x = new(2, 1);
  matrix * b = new(2, 1);
  matrix * l = new(2, 2);
  matrix * u = new(2, 2);
  matrix * y = new(2, 1);
  fill((double[4]){4, 3, 6, 3}, 4, a);
  fill((double[2]){1, 2}, 2, b);
  print(a);
  print(b);
  crout(a, l, u);
  print(l);
  print(u);
  solve_lower(l, y, b);
  print(y);
  solve_upper(u, x, y);
  print(x);
  return 0;
}
int8_t test_solver() {
  matrix * a = new(3, 3);
  matrix * x = new(3, 1);
  matrix * b = new(3, 1);
  fill((double[9]){7, 2, 1, 0, 3, -1, -3, 4, -2}, 9, a);
  fill((double[3]){21, 5, -1}, 3, b);
  print(a);
  print(b);
  solve(a, x, b);
  print(x);
  return 0;
}
