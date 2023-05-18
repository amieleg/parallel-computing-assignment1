#!/bin/sh

#SBATCH --account=csmpistud
#SBATCH --ntasks=4
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=8
#SBATCH --threads-per-core=1
#SBATCH --partition=csmpi_short
#SBATCH --time=00:05:00
#SBATCH --output=hello_mpi_4.out

# Compile on the machine, not the head node
make bin/hello_world_mpi
for N in 625 625000 625000000; do
    #TODO IMPORTANT change omp to mpi
    mpirun bin/hello_world_mpi > results/stencil_mpi_$N.txt
done
