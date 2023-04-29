#!/bin/sh

#SBATCH --account=csmpistud
#SBATCH --cpus-per-task=32
#SBATCH --partition=csmpi_fpga_short
#SBATCH --time=00:05:00
#SBATCH --output=stencil_seq.out

# Compile on the machine, not the head node
make bin/stencil_seq
# 5 kB, 5 MB, 5 GB
# We have to divide by 8 because a double takes up 8 bytes
for N in 625 625000 625000000; do
    for iters in 500 1000 1500 2000; do
        OMP_NUM_THREADS="1" bin/stencil_seq 10000 100 0 > results/stencil_seq_"$N,$iters".txt
    done
done
