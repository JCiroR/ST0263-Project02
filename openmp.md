# Word Count con OpenMP

## Oportunidades de paralelización

En el problema de generar un indice invertido encontramos las siguientes oportunidades de paralelización: 

1. Los diferentes artículos se procesan en paralelo, pues aunque sus resultados deben ser agregados, el procesamiento es independiente entre ellos.

2. El texto de cada artículo también puede ser procesado en paralelo, asignando el conteo de cierta cantidad de palabras a cada hilo.

3. Tokenizar el texto de cada artículo (para posteriormente contar las palabras), puede paralelizarse de manera que cada artículo sea divido en tokens por un hilo diferente.

4. Encontrar los artículos con más ocurrencias de una palabra puede divirse en batches, combinando al final los resultados de estos para responder la query. 

## Computos paralelizados

Dentro de lo anterior se escoge parelelizar solo el procesamiento de los diferentes artículos, por las siguientes razones: 

1. A nivel de lenguaje de programación, las estructuras de datos complejas son dificiles de paralelizar, tanto por dificultades dividiendolas como porque no todas sus operaciones son thread-safe.

2. El overhead de usar múltiples hilos hace que paralelizar operaciones muy livianas en procesamiento no valga la pena. 

3. Si paralelizando bloques grandes del programa ya se hace uso del 100% del poder de computo disponble, paralelizar aún más dentro de esos bloques solo agrega overhead y no provee beneficio. 

## Solución propuesta

explicar solucion

### Dificultades


## Analisis de resultados

