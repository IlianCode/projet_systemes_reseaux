all:main client


fonctions_serveur.o: fonctions_serveur.c fonctions_serveur.h
	gcc -c fonctions_serveur.c fonctions_serveur.h -Wall

main.o: main.c fonctions_serveur.c
	gcc -c main.c fonctions_serveur.c -Wall 

main: main.o fonctions_serveur.o
	gcc -o main main.o fonctions_serveur.o -Wall

fonctions_client.o: fonctions_client.c fonctions_client.h
	gcc -c fonctions_client.c fonctions_client.h -Wall

client.o: client.c fonctions_client.c
	gcc -c client.c fonctions_client.c -Wall 

client: client.o fonctions_client.o
	gcc -o client client.o fonctions_client.o -Wall


clean:
	rm -f *.o




