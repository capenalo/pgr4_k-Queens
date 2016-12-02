#!/bin/bash
mpirun -np $1 --hostfile hostfile pi_MPI $2
