import subprocess
from argparse import ArgumentParser
from pathlib import Path


def main():
    parser = ArgumentParser()

    parser.add_argument("--num_threads", "-nt", type=int, required=True)
    parser.add_argument("--problem_dimension", "-pdim", type=int, required=True)

    ARGS = parser.parse_args()

    out_path = Path("./src/bin")
    if not out_path.exists():
        out_path.mkdir(parents=True)

    parallel = True if ARGS.num_threads > 1 else False

    if parallel:
        pass
    else:
        pass


if __name__ == "__main__":
    main()
