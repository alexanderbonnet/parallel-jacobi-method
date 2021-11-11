CC = gcc
CFLAGS = -std=c99 -Wall -fopenmp -O3
UTILS = src/utils.c src/copy.c src/linalg.c

compile-seq:
	${CC} ${CFLAGS} src/be_hpc_seq.c ${UTILS} -o src/bin/be_hpc_seq.o

run-seq:
	./src/bin/be_hpc_seq.o

compile-par:
	${CC} ${CFLAGS} src/be_hpc_parallel.c ${UTILS} -o src/bin/be_hpc_parallel.o

run-par:
	./src/bin/be_hpc_parallel.o

clean-bin:
	rm -rf ./src/bin/*

clean-code:
	isort . && black . && flake8 .
