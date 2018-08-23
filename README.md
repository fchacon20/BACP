Compilación y uso:

make
./MMAS instancia ants alfa beta iterations maxPh seed

Donde:
- MMAS es el nombre del archivo
- instancia es un archivo txt con la información necesaria para utilizar el programa
- ants es la cantidad de hormigas
- alfa es la importancia que tienen las feromonas
- beta es la importancia que tiene la visibilidad
- iterations es la cantidad de iteraciones de MMAS
- maxPh es la cantidad máxima de feromonas
- seed es la semilla a utilizar, si es 0, se usa el reloj del computador

Ejemplo:
./MMAS bacp8.txt 30 1 2 1000 50 0

Makefile viene con 'make clean' para eliminar archivos sobrantes.
