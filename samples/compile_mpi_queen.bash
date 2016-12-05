#!/bin/bash
mpicc -std=c99 cpuTimes.c mpi_queen.c -o mpi_queen
