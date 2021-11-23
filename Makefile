programa: main.o funciones.o
	gcc -o programa main.o funciones.o
main.o: main.c funciones.c
	gcc -c main.c
funciones.o: funciones.c funciones.o 
	gcc -c funciones.c 
clean: 
	rm -f programa *.o