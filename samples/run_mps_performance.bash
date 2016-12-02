#!/bin/bash
mpicc mps_performance.c -o mps_performance
mpirun -np 2 --host eos01,eos02 mps_performance
