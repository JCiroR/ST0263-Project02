# Script para tests de OpenMP + MPI
export OMP_DISPLAY_ENV='true'
array=(2 3) #Diferentes numeros de threads
for sz in "${array[@]}"
do
    export OMP_NUM_THREADS="$sz"
    time mpirun -f hosts -np 3 ./word_count_mpi
done
