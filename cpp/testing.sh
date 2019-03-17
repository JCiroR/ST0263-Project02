# Script para tests de OpenMP
export OMP_DISPLAY_ENV='true'
array=(1 2 3 4 8 12 16 24) #Diferentes numeros de threads
for sz in "${array[@]}"
do
    export OMP_NUM_THREADS="$sz"
    time ./word_count_openmp
done