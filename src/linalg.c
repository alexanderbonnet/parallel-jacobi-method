#include "linalg.h"

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define ELEMENT double

ELEMENT squared_norm_of_diff(ELEMENT *vect_a, ELEMENT *vect_b, int size) {
    ELEMENT sum = 0, tmp = 0;
    for (int i = 0; i < size; i++) {
        tmp = vect_a[i] - vect_b[i];
        sum += tmp * tmp;
    }
    return sum;
}

ELEMENT squared_norm_of_diff_parallel(ELEMENT *vect_a, ELEMENT *vect_b,
                                      int size) {
    ELEMENT sum = 0;
#pragma omp parallel for shared(vect_a, vect_b, size) default(none) reduction(+: sum)
    for (int i = 0; i < size; i++) {
        ELEMENT tmp = vect_a[i] - vect_b[i];
        sum += tmp * tmp;
    }
    return sum;
}
