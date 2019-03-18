# ST0263-Project02 - Analítica de texto.
## Integrantes y sus contribuciones

### Juan José Suárez Estrada - jsuare32@eafit.edu.co
Declaro que participé en las siguientes etapas del proyecto, y que todas estas son de autoría propia:
* Análisis del problema bajo la metodología PCAM
* Analisis del diseño del programa con multihilos usando OpenMP
* Programación de scripts de preprocesado de datos en Python
* Programación de algoritmo con OpenMP
* Programación de algoritmo con MPI y OpenMP

### Juan Manuel Ciro Restrepo - jcirore@eafit.edu.co
Declaro que participé en las siguientes etapas del proyecto, y que todas estas son de autoría propia:
* Análisis del problema bajo la metodología PCAM
* Analisis del diseño del programa con multihilos usando OpenMP
* Programación de algoritmo serial en C++
* Programación de algoritmo con MPI y OpenMP

## Compilación y ejecución
Para compilar ir a la carpeta /cpp y correr el comando de compilación correspondiente. Para ejecutar las versiones con OpenMP
es necesario configurar el número de threads que se usará:

    $ export OMP_NUM_THREADS=2 // Usar dos threads

### Versión serial
    $ cd cpp
    $ make serial
    $ ./word_count
    
### Versión con OpenMP
    $ cd cpp
    $ make openmp
    $ ./word_count_openmp

### Versión con MPI y OpenMP
    $ cd cpp
    $ make mpi
    $ mpirun -f hosts -np 3 ./word_count_mpi
    
 ## Análisis de resultados
 
## Videos sustentación del proyecto
[Juan José Suárez Estrada](https://www.google.com)

[Juan Manuel Ciro Restrepo](https://www.google.com)

