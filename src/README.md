### K-Queens MPI
# Compile
```bash
$> mpicc k_queens_mpi.c -o k_queens_mpi -lm
```

# Run
```bash
$> mpirun -np <nodes> --hostfile hostfile k_queens_mpi <board_size>
```
