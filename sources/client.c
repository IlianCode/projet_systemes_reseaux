#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>

#include <ctype.h>


#include "../headers/fonctions_client.h"

int main(int argc, char *argv[])
{
    // create the client part of the tcpi ip server
    struct sockaddr_in addrClient;
    int socketClient;
    const char *namehost = argv[2];
    // def the structure of the server
    struct hostent *infos_server = NULL;
    int port = atoi(argv[1]);

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == -1)
    {
        printf("Error creating socket");
        exit(-1);
    }

    infos_server = gethostbyname(namehost);
    if (infos_server == NULL)
    {
        printf("Error getting host by name");
        exit(-1);
    }

    // give value to the addrClient
    addrClient.sin_family = AF_INET;
    memcpy(&addrClient.sin_addr.s_addr, infos_server->h_addr, sizeof(u_long));
    addrClient.sin_port = htons(port);

    // connect to the server
    if (connect(socketClient, (struct sockaddr *)&addrClient, sizeof(addrClient)) == -1)
    {
        printf("Error connecting to server");
        exit(-1);
    }
    else
    {
        printf("Connected to server \n");
    }
    //================================================================================================
    bool isRunning = true;

    // debut reception données et envoie de reponse
    while (isRunning)
    {
        // affiche le menu utilisateur
        displayClientMenu();

        char checkTypeQuery;
        int typeQuery = 0;

        // check if typequery is a number then send it to the server
        do
        {
            printf("Entrez le numero de la requete souhaité (1/2/3/4):\n");
            scanf("%c", &checkTypeQuery);
        } while (checkTypeQuery != '1' && checkTypeQuery != '2' && checkTypeQuery != '3' && checkTypeQuery != '4');

        typeQuery = atoi(&checkTypeQuery);
        write(socketClient, &typeQuery, sizeof(int));

         
        handleClientQuery(socketClient, typeQuery);
    
     //demande a l'utilisateur si il veut continuer ou non
        isRunning = askEnd(socketClient);

        // envoie 0 au serveur si isRunning = true sinon envoie 1 au serveur
        int endClient = 0;
        if (isRunning == true)
        {
            endClient = 0;
        }
        else
        {
            endClient = 1;
        }
        write(socketClient, &endClient, sizeof(int));
    }
    //=============

    close(socketClient);
    return 0;
}
