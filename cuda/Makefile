CC = nvcc
CFLAGS = -O3 -arch=sm_30

compile-cuda:
	${CC} ${CFLAGS} -o src/bin/jacobi src/jacobi.cu

run-cuda:
	src/bin/jacobi
