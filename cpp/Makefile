openmp: wordCount_openmp.cpp
	g++ $^ -o word_count_openmp -fopenmp -O2

mpi: word_count_mpi.cpp
	mpic++ -O2 -w $^ -o word_count_mpi -fopenmp -std=c++11

serial: wordCount.cpp
	g++ $^ -o word_count -std=c++11 -O2