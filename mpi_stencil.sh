#!/bin/sh

NODES=1
while [ $NODES -le 8 ]
do
    NTASKS=$(( 8 * NODES ))
    
    sed "s/NODES/$NODES/g" mpi_stencil.sh.template > mpi_stencil_"$NTASKS".sh
    sed -i "s/NTASKS/$NTASKS/g" mpi_stencil_"$NTASKS".sh
    
    sbatch mpi_stencil_"$NTASKS".sh

    rm mpi_stencil_"$NTASKS".sh

    NODES=$(( 2 * NODES ))
done
