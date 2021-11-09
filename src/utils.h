#ifndef UTILS_HEADER_FILE
#define UTILS_HEADER_FILE

#define ELEMENT double

int omp_thread_count();

ELEMENT mean(ELEMENT *x_array, int size);

ELEMENT *init_x(int size);

ELEMENT *init_b(int size);

ELEMENT *init_a(int size);

#endif
