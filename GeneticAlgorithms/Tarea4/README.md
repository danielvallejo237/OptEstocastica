
Compilar usando `make`.
Ejecutar con el siguiente comando
```
./main.o <Scenario> <layout> <output file>
```
`<Scenario>` Representa la instancia.
`<layout>` Es el archivo con la solución propuesta.
`<output file>` Es el nombre del archivo de salida.

Ejemplo de uso:
```
./main.o ./Scenarios/98.xml layout2.txt ./out.txt
```

El formato del archivo solución es como sigue:

AnchoDeMalla AltoDeMalla
NumeroDeTurbinas
Arreglo de 0 y 1.

Se puede ver los archivos layout como ejemplo.


En caso de que se quieran usar las funciones del evaluador, se encuentra un ejemplo en main para un arreglo de enteros compuesto de 0s y 1s (descomentar esa sección).
Si la solución sobrepasa el número de turbinas especificado por la instancia la solucion se vuelve infactible. Lo mismo ocurre si la distancia entre las turbinas es  menor a 10xR, en donde R es el Radio de las turbinas. Para este caso, el radio está en 38.5m.
