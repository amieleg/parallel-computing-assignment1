#!/bin/sh

#SBATCH --account=csmpistud
#SBATCH --cpus-per-task=32
#SBATCH --partition=csmpi_fpga_short
#SBATCH --time=00:05:00
#SBATCH --output=stencil_seq.out

# Compile on the machine, not the head node
make bin/stencil_seq
for N in 10 20 30 40 50 60; do
    for iters in 1 2 3 4 5 6; do
        OMP_NUM_THREADS="1" bin/stencil_seq 10000 100 0 > results/stencil_seq_"$N,$iters".txt
    done
done
