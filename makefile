all:main 

fonctions_serveur.o:fonctions_serveur.c fonctions_serveur.h
	gcc -c fonctions_serveur.c fonctions_serveur.h -Wall

main.o:main.c fonctions_serveur.c
	gcc -c main.c fonctions_serveur.c -Wall 

main: main.o fonctions_serveur.o
	gcc -o main main.o fonctions_serveur.o -Wall

clean:
	rm -f *.o
	



