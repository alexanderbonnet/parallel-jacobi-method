#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define ELEMENT double

int omp_thread_count() {
  int n = 0;
#pragma omp parallel reduction(+ : n)
  n += 1;
  return n;
}

ELEMENT mean(ELEMENT *x_array, int size) {
  ELEMENT mean = 0;
  for (int i = 0; i < size; i++) {
    mean += x_array[i];
  }
  mean = mean / size;
  return mean;
}

ELEMENT *init_x(int size) {
  ELEMENT *x_vect = malloc(sizeof(ELEMENT) * size);
  for (int i = 0; i < size; i++) {
    x_vect[i] = 1;
  }
  return x_vect;
}

ELEMENT *init_b(int size) {
  ELEMENT *b_vect = malloc(sizeof(ELEMENT) * size);
  for (int i = 0; i < size; i++) {
    b_vect[i] = 6;
  }
  return b_vect;
}

ELEMENT *init_a(int size) {
  ELEMENT *a_matrix = malloc(sizeof(ELEMENT) * size * size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (j == i) {
        a_matrix[i + j * size] = 2 * size + 1;
      } else {
        a_matrix[i + j * size] = 1;
      }
    }
  }
  return a_matrix;
}
