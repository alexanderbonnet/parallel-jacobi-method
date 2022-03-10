#ifndef COPY_HEADER_FILE
#define COPY_HEADER_FILE

#define ELEMENT double

// TODO: remove copy
ELEMENT *copy(ELEMENT *vect, int size);

ELEMENT *copy_parallel(ELEMENT *vect, int size);

void copy_inplace(ELEMENT *origin_vect, ELEMENT *dest_vect, int size);

void copy_inplace_parallel(ELEMENT *origin_vect, ELEMENT *dest_vect, int size);

#endif