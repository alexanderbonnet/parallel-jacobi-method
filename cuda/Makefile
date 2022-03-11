CC = nvcc
CFLAGS = -O3 -arch=sm_30

compile-cuda:
	${CC} ${CFLAGS} -o src/bin/jacobi.o src/jacobi.cu

run-cuda:
	src/bin/jacobi.o

clean-bin:
	rm -rf ./src/bin/*
