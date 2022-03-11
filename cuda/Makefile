CC = nvcc
CFLAGS = -O3 -arch=sm_30

compile-cuda:
	${CC} ${CFLAGS} -o src/bin/jacobi_cuda.o src/jacobi_cuda.cu

run-cuda:
	src/bin/jacobi_cuda.o

clean-bin:
	rm -rf ./src/bin/*
