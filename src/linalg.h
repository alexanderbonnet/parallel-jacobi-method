#ifndef LINALG_HEADER_FILE
#define LINALG_HEADER_FILE

#define ELEMENT double

ELEMENT squared_norm_of_diff(ELEMENT *vect_a, ELEMENT *vect_b, int size);

ELEMENT squared_norm_of_diff_parallel(ELEMENT *vect_a, ELEMENT *vect_b, int size);

#endif
