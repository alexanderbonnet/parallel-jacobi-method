# parallel-jacobi-method

An implementation of the iterative Jacobi algorithm for solving strictly diagonally dominant linear systems. Implemented during a lab session at ISAE-Supaéro.

The C-language source code is contained is src/ and includes sequential and parallel implementations (using OpenMP).
Simple Python bindings compile and consecutively run the program with desired arguments.

```shell
python -m python_binding.jacobi_method --num_threads (an int) --problem dimension (an int)
```
