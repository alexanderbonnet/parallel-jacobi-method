#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define ELEMENT double

ELEMENT *copy(ELEMENT *vect, int size) {
    ELEMENT *vect_new = malloc(sizeof(ELEMENT) * size);
    for (int i = 0; i < size; i++) {
        vect_new[i] = vect[i];
    }
    return vect_new;
}

ELEMENT *copy_parallel(ELEMENT *vect, int size) {
    ELEMENT *vect_new = malloc(sizeof(ELEMENT) * size);
#pragma omp parallel for shared(vect, vect_new, size) default(none)
    for (int i = 0; i < size; i++) {
        vect_new[i] = vect[i];
    }
    return vect_new;
}

void copy_inplace(ELEMENT *origin_vect, ELEMENT *dest_vect, int size) {
    for (int i = 0; i < size; i++) {
        dest_vect[i] = origin_vect[i];
    }
}

void copy_inplace_parallel(ELEMENT *origin_vect, ELEMENT *dest_vect, int size) {
#pragma omp parallel for shared(origin_vect, dest_vect, size) default(none)
    for (int i = 0; i < size; i++) {
        dest_vect[i] = origin_vect[i];
    }
}