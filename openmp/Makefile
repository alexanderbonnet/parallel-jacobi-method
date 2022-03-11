CC = gcc
CFLAGS = -std=c99 -Wall -fopenmp -O3
UTILS = src/utils.c
INCLUDE = include

compile-seq:
	${CC} ${CFLAGS} src/jacobi_seq.c ${UTILS} -I ${INCLUDE} -o src/bin/jacobi_seq.o

run-seq:
	./src/bin/jacobi_seq.o

compile-par:
	${CC} ${CFLAGS} src/jacobi_parallel.c ${UTILS} -I ${INCLUDE} -o src/bin/jacobi_parallel.o

run-par:
	./src/bin/jacobi_parallel.o

clean-bin:
	rm -rf ./src/bin/*

clean-code:
	isort . && black . && flake8 .
