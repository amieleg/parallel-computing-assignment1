#!/bin/sh

NODES=1
while [ $NODES -le 8 ]
do
    TASKSPERNODE=1
    while [ $TASKSPERNODE -le 8 ]
    do
        NTASKS=$((TASKSPERNODE * NODES ))
        info="${NODES} ${TASKSPERNODE}"
        echo $info >> results/RESULTS.txt 
        sed "s/NODES/$NODES/g" run_mpi.sh.template > run_mpi_"$NTASKS".sh
        sed -i "s/NTASKS/$NTASKS/g" run_mpi_"$NTASKS".sh
        # ./ -> sbatch or sbatch -> ./
        # chmod u+x,g+wx * # Remove this on the remote
        sbatch run_mpi_"$NTASKS".sh
    
        rm run_mpi_"$NTASKS".sh
        TASKSPERNODE=$(( 2 * TASKSPERNODE ))
    done
    NODES=$(( 2 * NODES ))
done
