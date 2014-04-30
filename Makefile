#	Makefile recursivo
#	Autor : Fernando Vergara Díaz
#	Modificación: Mauricio Guarda


SIN_PARAMETROS:
	echo "problemas de ejecucion"
	make all

# Permite compilar a todos los problemas del presente directorio
# TSP - QAP
# ejecutar: $make all

all:

	cd TSP_PERM; make
	cd QAP_PERM; make

# Permite limpiar ejecutables y todo soble el proceso de compilacion
# ejecutar: $make distclean

distclean:
	cd TSP_PERM; make clean
	cd QAP_PERM; make clean
	rm EJECUTABLE*

# Permite limpiar solamente todo el proceso de compilacion
# ejecutar: $make clean

clean:
	cd TSP_PERM; make clean
	cd QAP_PERM; make clean

# Permite compilar al problema TSP
# ejecutar: $make tsp

tsp:
	cd TSP_PERM; make

# Permite compilar al problema QAP
# ejecutar: $make qap

qap:
	cd QAP_PERM; make

