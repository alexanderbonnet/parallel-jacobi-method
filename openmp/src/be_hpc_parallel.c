#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linalg.h"
#include "utils.h"

#define ELEMENT double

void increment_x(ELEMENT *x_vect, ELEMENT *x_tmp, ELEMENT *a_matrix,
                 ELEMENT *b_vect, int size) {
#pragma omp parallel for shared(size, a_matrix, x_tmp, x_vect, \
                                b_vect) default(none)
    for (int i = 0; i < size; i++) {
        ELEMENT sum = 0;
        for (int j = 0; j < size; j++) {
            if (i != j) {
                sum += a_matrix[i * size + j] * x_tmp[j];
            }
        }
        x_vect[i] = (b_vect[i] - sum) / a_matrix[i * size + i];
    }
}

int solve_with_jacobi(ELEMENT *x_new, ELEMENT *x_old, ELEMENT *a_matrix,
                      ELEMENT *b_vect, int size, ELEMENT epsilon) {
    int nit = 0;
    ELEMENT eps_2 = epsilon * epsilon;
    ELEMENT crit = eps_2 + 1;

    while (crit > eps_2) {
        increment_x(x_new, x_old, a_matrix, b_vect, size);
        crit = squared_norm_of_diff_parallel(x_new, x_old, size);
        memcpy(x_old, x_new, size * sizeof(ELEMENT));
        nit += 1;
    }

    return nit;
}

int main(int argc, char *argv[]) {
    int size = 0;
    int num_threads = 0;

    if (argc == 1) {
        size = 2048;
        num_threads = 4;
    } else if (argc == 3) {
        size = atoi(argv[1]);
        num_threads = atoi(argv[2]);
    } else {
        printf("ERROR - need two arguments - exiting \n");
        exit(0);
    }

    int num_executions = 1;

    // number of iterations of the algorithm and result
    int nit = 0;
    ELEMENT result = 0;

    ELEMENT t1 = 0, t2 = 0, avg_time = 0;
    ELEMENT eps = 1e-6;

    ELEMENT *a_matrix = init_a(size);
    ELEMENT *x_init = init_x(size, 1);
    ELEMENT *b_vect = init_x(size, 6);

    ELEMENT *execution_times = malloc(sizeof(ELEMENT) * num_executions);

    printf(
        "running parallel version of iterative Jacobi algorithm with size = "
        "%d\n",
        size);

    // set number of threads for parallel execution
    omp_set_num_threads(num_threads);

    // verify the effective number of threads
    printf("Number of threads: %d\n", omp_thread_count());

    for (int i = 0; i < num_executions; i++) {
        ELEMENT *x_old = malloc(size * sizeof(ELEMENT));
        ELEMENT *x_new = malloc(size * sizeof(ELEMENT));

        memcpy(x_old, x_init, size * sizeof(ELEMENT));
        memcpy(x_new, x_init, size * sizeof(ELEMENT));

        t1 = omp_get_wtime();
        nit = solve_with_jacobi(x_new, x_old, a_matrix, b_vect, size, eps);
        t2 = omp_get_wtime();

        execution_times[i] = t2 - t1;
        printf("%.10f \n", t2 - t1);

        result = x_new[0];

        free(x_old);
        free(x_new);
    }

    avg_time = mean(execution_times, num_executions);

    printf("number of iterations = %d \n", nit);
    printf("execution time = %.10fs \n", avg_time);
    printf("\n");
    printf("our result = %.10f \n", result);
    printf("theoretical result = %.10f \n", 2.0 / size);

    free(execution_times);
    free(a_matrix);
    free(b_vect);
    free(x_init);

    return 0;
}
