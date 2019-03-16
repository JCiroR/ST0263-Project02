
# PCAM
## Particionamiento
### De tareas

* Crear una tabla global (palabra, [(artículo, freq), … ])
    * Para cada articulo generar una tabla de frecuencias (palabra, freq)
        * Dividir el contenido en tokens
        * Para cada palabra del artículo
            * Agregar si hace falta
            * Incrementar la frecuencia de la palabra en la tabla
        * Añadir la información de la tabla de frecuencias del artículo a la global
            * Agregar palabra si hace falta
            * Agregar la tupla (id articulo, titulo, frecuencia)
    * Para cada palabra encontrar los K artículos en los que más aparece
        * Buscar en la tabla global la fila correspondiente a la palabra
        * Extraer los K artículos con más ocurrencias de la fila
        * Ordenar los artículos seleccionados.

### De datos
* Los datos a procesar pueden dividirse por artículo. Para  generar cada tabla de frecuencias se necesita la información de un solo artículo.

* Para la tabla global se necesitan las tablas de frecuencia parciales de cada artículo.

* Para cada artículo se deben contar las ocurrencias de cada palabra que lo compone. La mínima unidad en que puede computarse eso es una palabra, pero podría realizarse en grupos más grandes.

## Comunicación
[logo]:https://i.ibb.co/X5wDyQc/Comunicacion.png"  "Comunicación"

![alt text][logo]

## Aglomeración 

* Los artículos se procesaran por batches del mismo tamaño. Para cada artículo, las tareas de separar y contar palabras se realizan juntas.

* En el procesamiento de queries, extraer los K artículos con más ocurrencias de una palabra, y ordenarlos, también se aglomeran.

## Mapeo

* Respecto a datos, el dataset completo de articulos se divide en tantos batches como workers estén disponibles. Cada worker se encarga de procesar su batch artículo a artículo, dividiendo este proceso tantos threads como procesadores disponibles tenga el worker.

* Teniendo en cuenta lo anterior, tenemos que las tareas para crear la tabla global se agrupan por artículo, es decir: todas las tareas que involucran un un artículo son procesadas por el mismo worker y thread. estas tareas son:

    * Dividir el artículo en tokens.
    * Contar palabras.
    * Juntar contadores en una tabla parcial.
    * Actualizar la tabla global con la información de la tabla parcial.

* Una vez se genera la tabla global, resta procesar las queries. Cada worker procesa una query en su batch de artículos, enviando los K con más ocurencias al master. El master, además de procesar su batch, _reduce_ las respuestas de todos los workers y responde la query.
