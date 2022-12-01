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

void createProcessClient(int socketClient, int socketServer);
void handleClient(int socketClient);
void end_of_child();
void sendResponseToQuery(int clientAnswer, int socketClient);

int searchSize(char *str);
void sendMessage(int socketClient, char *message);
void queryTreatment(int clientAnswer, int clientReference, int socketClient);


int main(int argc, char *argv[])
{
    int socketServer, socketClient;
    struct sockaddr_in adresse, struct2;
    int binded;
    int clientAddresseLen;
    char *buffer[1024];
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
    clientAddresseLen = sizeof(struct2);

    //start of the server
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





//function
void createProcessClient(int socketClient, int socketServer)
{
    switch (fork())
    {
    case -1:
        printf("Error creating process");
        exit(1);
    case 0:
        close(socketServer);

        handleClient(socketClient);
        printf("Client disconnected successfully\n");
        exit(0);
    default:
        close(socketClient);
    }
}

// function to listen to the client query
void handleClient(int socketClient)
{
    int endClient = 0;
    printf("Client connected\n");

    while (endClient != 1)
    {

        // receive the query type of the client
        int clientAnswer = 0;
        read(socketClient, &clientAnswer, sizeof(int));
        //================================


        //lecture reference client int : 
        int clientReference = 0;
        read(socketClient, &clientReference, sizeof(int));
        //================================

        queryTreatment(clientAnswer, clientReference, socketClient);

        // lis la reponse du client
        read(socketClient, &endClient, sizeof(int));
    }

    // close the socket
    close(socketClient);
}

void end_of_child()
{
    wait(NULL);
}


int searchSize(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i + 1;
}

void sendMessage(int socketClient, char *message)
{
    int size = searchSize(message) * sizeof(char);
    printf("size of message: %d \n", size);

    write(socketClient, &size, sizeof(int));

    write(socketClient, *&message, size);
}






// traitement des requete en fonction de clientAnswer
void queryTreatment(int clientAnswer, int clientReference, int socketClient)
{

    switch (clientAnswer)
    {
    case 1:
        //transform clientReference to string to search in the file
        char clientReferenceString[4];
        sprintf(clientReferenceString, "%d", clientReference);

        char line[1024];
        FILE *fp = fopen("bdd_bibliotheque.txt", "r");
        // read the open file
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            if (strstr(line, clientReferenceString) != NULL)
            {
               
                    char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

                    strcpy(cRef, strtok(line, "#"));
                    strcpy(cAuthor, strtok(NULL, "#"));
                    strcpy(cTitle, strtok(NULL, "#"));
                    strcpy(cType, strtok(NULL, "#"));
                    strcpy(cNbPages, strtok(NULL, "#"));
                    strcpy(cRate, strtok(NULL, "#"));


                    if (atoi(cRef) ==clientReference)
                    {
                       

                        char res[560];
                        strcat(res, cRef);
                        char resAuthor[50] = "&";
                        strcat(res, resAuthor);
                        strcat(res, cAuthor);
                        char resTitle[50] = "&";
                        strcat(res, resTitle);
                        strcat(res, cTitle);
                        char resType[50] = "&";
                        strcat(res, resType);
                        strcat(res, cType);
                        char resNbPages[50] = "&";
                        strcat(res, resNbPages);
                        strcat(res, cNbPages);
                        char resRate[50] = "&";
                        strcat(res, resRate);
                        strcat(res, cRate);
                       

                        printf("%s \n", res);

                        sendMessage(socketClient, res);
                        break;
                    }
                    /*else
                    {
                        char *res = "Aucune référence pour votre livre n'est trouvé.";

                        sendMessage(socketClient, res);
                    }*/
                
            }
        }
        break;
    }
}

