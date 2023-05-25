#!/bin/sh

#SBATCH --account=csmpistud
#SBATCH --ntasks=16
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=8
#SBATCH --threads-per-core=1
#SBATCH --partition=csmpi_short
#SBATCH --time=00:05:00
#SBATCH --output=hello_mpi_16.out

# Compile on the machine, not the head node
make bin/stencil_mpi

#echo "running like a madman" > results/stencil_mpi_16.txt
# N ITERS SHOW
for N in 100 1000 10000 100000; do
    # REMOVE -N NTAKS and --oversubscribe on the SLURM!!!!
    mpirun -N 16 --oversubscribe bin/stencil_mpi $N 1 0 > results/temp_results.txt
    echo $N 16 > results/temp_params.txt
    times=`cat results/temp_results.txt`
    row="16 ${N} ${times}"
    echo $row >> results/RESULTS.txt 
done
