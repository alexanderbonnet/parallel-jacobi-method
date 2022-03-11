#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 2048
#define THREADS_PER_BLOCK 128
#define N_BLOCKS (N / THREADS_PER_BLOCK + 1)
#define ELEMENT double

ELEMENT mean(ELEMENT *x_array, int size) {
    ELEMENT mean = 0;
    for (unsigned int i = 0; i < size; i++) {
        mean += x_array[i];
    }
    mean = mean / size;
    return mean;
}

ELEMENT *init_x(unsigned int size, ELEMENT value) {
    ELEMENT *x_vect = (ELEMENT *)malloc(sizeof(ELEMENT) * size);
    for (unsigned int i = 0; i < size; i++) {
        x_vect[i] = value;
    }
    return x_vect;
}

ELEMENT *init_a(unsigned int size) {
    ELEMENT *a_matrix = (ELEMENT *)malloc(sizeof(ELEMENT) * size * size);
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < size; j++) {
            if (j == i) {
                a_matrix[i + j * size] = 2 * size + 1;
            } else {
                a_matrix[i + j * size] = 1;
            }
        }
    }
    return a_matrix;
}

__global__ void warm_up_gpu() {
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    ELEMENT ia, ib;
    ia = ib = 0.0f;
    ib += ia + tid;
}

__global__ void criterion(ELEMENT *a, ELEMENT *b, ELEMENT *crit) {
    __shared__ ELEMENT temp[THREADS_PER_BLOCK];

    *crit = 0;
    unsigned int index = threadIdx.x;
    ELEMENT sum = 0;
    ELEMENT diff = 0;
    for (unsigned int i = index; i < N; i += THREADS_PER_BLOCK) {
        diff = a[i] - b[i];
        sum += diff * diff;
    }
    temp[index] = sum;

    __syncthreads();
    for (unsigned int size = THREADS_PER_BLOCK / 2; size > 0; size /= 2) {
        if (index < size) {
            temp[index] += temp[index + size];
        }
        __syncthreads();
    }
    if (index == 0) {
        *crit = temp[0];
    }
}

__global__ void increment_x(ELEMENT *x_new, ELEMENT *x_old, ELEMENT *a_mat,
                            ELEMENT *b_vec) {
    unsigned int index = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int indexN = N * index;

    if (index < N) {
        ELEMENT sum = 0;
        for (unsigned int j = 0; j < N; j++) {
            if (index != j) {
                sum += a_mat[indexN + j] * x_old[j];
            }
        }
        x_new[index] = (b_vec[index] - sum) / a_mat[indexN + index];
    }
}

unsigned int solve_with_jacobi(ELEMENT *x_init, ELEMENT *a_mat, ELEMENT *b_vec,
                               ELEMENT epsilon) {
    unsigned int nit = 0;
    ELEMENT eps_2 = epsilon * epsilon;
    ELEMENT crit = eps_2 + 1;

    ELEMENT *dev_a, *dev_b, *dev_x_old, *dev_x_new, *dev_crit;

    cudaMalloc((void **)&dev_a, N * N * sizeof(ELEMENT));
    cudaMalloc((void **)&dev_b, N * sizeof(ELEMENT));
    cudaMalloc((void **)&dev_x_old, N * sizeof(ELEMENT));
    cudaMalloc((void **)&dev_x_new, N * sizeof(ELEMENT));
    cudaMalloc((void **)&dev_crit, sizeof(ELEMENT));

    // copy inputs to device
    cudaMemcpy(dev_a, a_mat, N * N * sizeof(ELEMENT), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b_vec, N * sizeof(ELEMENT), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_x_old, x_init, N * sizeof(ELEMENT), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_x_new, x_init, N * sizeof(ELEMENT), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_crit, &crit, sizeof(ELEMENT), cudaMemcpyHostToDevice);

    while (crit > eps_2) {
        increment_x<<<N_BLOCKS, THREADS_PER_BLOCK>>>(dev_x_new, dev_x_old,
                                                     dev_a, dev_b);
        criterion<<<1, THREADS_PER_BLOCK>>>(dev_x_new, dev_x_old, dev_crit);
        cudaMemcpy(dev_x_old, dev_x_new, N * sizeof(ELEMENT),
                   cudaMemcpyDeviceToDevice);
        cudaMemcpy(&crit, dev_crit, sizeof(ELEMENT), cudaMemcpyDeviceToHost);
        nit += 1;
    }
    cudaMemcpy(x_init, dev_x_new, N * sizeof(ELEMENT), cudaMemcpyDeviceToHost);

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_x_old);
    cudaFree(dev_x_new);
    cudaFree(dev_crit);

    return nit;
}

int main(int argc, char *argv[]) {
    unsigned int num_executions = 20;

    unsigned int nit = 0;
    ELEMENT result = 0;

    ELEMENT eps = 1e-6;

    ELEMENT *a_mat = init_a(N);
    ELEMENT *x_init = init_x(N, 1);
    ELEMENT *b_vec = init_x(N, 6);

    ELEMENT *execution_times =
        (ELEMENT *)malloc(sizeof(ELEMENT) * num_executions);

    struct timeval t1, t2;
    ELEMENT time = 0;

    printf(
        "running iterative Jacobi algorithm with size = "
        "%d\n",
        N);

    warm_up_gpu<<<N / THREADS_PER_BLOCK, THREADS_PER_BLOCK>>>();

    for (unsigned int i = 0; i < num_executions; i++) {
        ELEMENT *x_solve = (ELEMENT *)malloc(N * sizeof(ELEMENT));
        memcpy(x_solve, x_init, N * sizeof(ELEMENT));

        gettimeofday(&t1, 0);

        nit = solve_with_jacobi(x_solve, a_mat, b_vec, eps);

        cudaDeviceSynchronize();
        gettimeofday(&t2, 0);

        time = 1000000.0 * (t2.tv_sec - t1.tv_sec) + t2.tv_usec - t1.tv_usec;
        time = time / 1000.0;

        execution_times[i] = time;
        printf("exec time = %f \n", time);

        result = x_solve[0];

        free(x_solve);
    }

    ELEMENT avg_time = mean(execution_times, num_executions);

    printf("number of iterations = %d \n", nit);
    printf("execution time = %.10fs \n", avg_time);
    printf("\n");
    printf("our result = %.10f \n", result);
    printf("theoretical result = %.10f \n", 2.0 / N);

    free(execution_times);
    free(a_mat);
    free(b_vec);
    free(x_init);

    return 0;
}
