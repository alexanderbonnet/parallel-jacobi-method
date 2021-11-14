import subprocess
from argparse import ArgumentParser
from pathlib import Path

CC = "gcc"
CFLAGS = "-std=c99 -Wall -fopenmp -O3"
UTILS = "src/utils.c src/copy.c src/linalg.c"
INCLUDE = "include"


def main():
    parser = ArgumentParser()

    parser.add_argument("--num_threads", "-nt", type=int, required=True)
    parser.add_argument("--problem_dimension", "-pdim", type=int, required=True)

    ARGS = parser.parse_args()

    if not Path("./src/bin").exists():
        Path("./src/bin").mkdir(parents=True)

    if ARGS.num_threads > 1:
        compile_cmd = [
            f"{CC} {CFLAGS}",
            f"src/be_hpc_parallel.c {UTILS}",
            f"-I {INCLUDE}",
            "-o src/bin/be_hpc_parallel.o",
        ]
        subprocess.run(" ".join(compile_cmd), shell=True)

        run_cmd = [
            "./src/bin/be_hpc_parallel.o",
            ARGS.problem_dimension,
            ARGS.num_threads,
        ]
        subprocess.run(" ".join(str(item) for item in run_cmd), shell=True)
    elif ARGS.num_threads == 1:
        compile_cmd = [
            f"{CC} {CFLAGS}",
            f"src/be_hpc_seq.c {UTILS}",
            f"-I {INCLUDE}",
            "-o src/bin/be_hpc_seq.o",
        ]
        subprocess.run(" ".join(compile_cmd), shell=True)

        run_cmd = [
            "./src/bin/be_hpc_seq.o",
            ARGS.problem_dimension,
        ]
        subprocess.run(" ".join(str(item) for item in run_cmd), shell=True)
    else:
        # explicitly handle error
        raise ValueError("num_threads must be striclty larger than 1")


if __name__ == "__main__":
    main()
