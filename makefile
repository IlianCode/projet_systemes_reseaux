all: ./bin/main ./bin/client

./bin/main: ./bin/main.o ./bin/fonctions_serveur.o
	gcc -o ./bin/main.o ./bin/fonctions_serveur.o -o ./bin/main -w

./bin/main.o: ./sources/main.c ./headers/fonctions_serveur.h
	gcc -c -Wall ./sources/main.c -o ./bin/main.o 

./bin/fonctions_serveur.o: ./sources/fonctions_serveur.c ./headers/fonctions_serveur.h
	gcc -c -Wall ./sources/fonctions_serveur.c -w -o ./bin/fonctions_serveur.o 

./bin/main: ./bin/main.o ./bin/fonctions_serveur.o
	gcc -o ./bin/main ./bin/main.o ./bin/fonctions_serveur.o 



## make pour le client
#
./bin/client: ./bin/client.o ./bin/fonctions_client.o
	gcc -o ./bin/client.o ./bin/fonctions_client.o -o ./bin/client -w


./bin/client.o: ./sources/client.c ./headers/fonctions_client.h
	gcc -c -Wall ./sources/client.c -o ./bin/client.o 

./bin/fonctions_client.o: ./sources/fonctions_client.c ./headers/fonctions_client.h
	gcc -c -Wall ./sources/fonctions_client.c -w -o ./bin/fonctions_client.o


./bin/client: ./bin/client.o ./bin/fonctions_client.o
	gcc -o ./bin/client ./bin/client.o ./bin/fonctions_client.o



