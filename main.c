#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#include "fonctions_serveur.c"


int main(int argc, char *argv[])
{
    int socketServer, socketClient;
    struct sockaddr_in adresse, struct2;
    int binded;
    int clientAddresseLen;
    struct sigaction killzombie;
    // create a tcpip server socket
    // create a listening socket
    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer == -1)
    {
        printf("Error creating socket");
        exit(1);
    }
    else
    {
        printf("Socket created successfully \n");
    }
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(9999);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);

    // prevent from zombies process
    killzombie.sa_handler = end_of_child;
    killzombie.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &killzombie, NULL);

    // attach the listening socket to an address
    binded = bind(socketServer, (struct sockaddr *)&adresse, sizeof(adresse));
    if (binded == -1)
    {
        printf("Error binding socket");
        exit(1);
    }
    else
    {
        printf("Socket binded successfully\n");
    }
    // start listenning on the socket
    if (listen(socketServer, 5) == -1)
    {
        printf("Error listening on socket");
        exit(1);
    }
    else
    {
        printf("Socket listening successfully\n");
    }
    clientAddresseLen = sizeof(adresse);

    // start of the server
    int n = 1;
    while (n == 1)
    {
        // listening for query from client
        socketClient = accept(socketServer, (struct sockaddr *)&struct2, &clientAddresseLen);
        if (socketClient == -1)
        {
            printf("Error accepting client");
            exit(1);
        }
        // if a connexion is established call the function createProcessClient
        createProcessClient(socketClient, socketServer);
    }
    return 0;
}
