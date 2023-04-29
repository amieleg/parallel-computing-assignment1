#!/bin/sh

#SBATCH --account=csmpistud
#SBATCH --cpus-per-task=32
#SBATCH --partition=csmpi_fpga_short
#SBATCH --time=00:05:00
#SBATCH --output=stencil_omp.out

# Compile on the machine, not the head node
make bin/stencil_seq

for P in 1 2 3 4 5 6; do
    OMP_NUM_THREADS="$P" bin/stencil_seq 10000 100 0 > results/stencil_seq_"$P".txt
done
