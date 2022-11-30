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
void queryTreatment(int clientAnswer, char *clientReference, int socketClient);


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

    // check if socker server is running
    

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
        printf("Client accepted successfully\n");
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
        printf("Message received from client: %d \n", clientAnswer);
        //================================

       // sendResponseToQuery(clientAnswer, socketClient);

        //lecture reference envoyé par le client
        int size = 0;
        read(socketClient, &size, sizeof(int));
        printf("size = %d \n", size);

        char clientReference[4];
        read(socketClient, &clientReference, size);
        printf("%s \n", clientReference);
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
/*
void sendResponseToQuery(int clientAnswer, int socketClient)
{
    switch (clientAnswer)
    {
    case 1:
        // le client choisis via reference de livre
        char *servAns1 = (char *)malloc((90) * sizeof(char));
        servAns1 = "Vous avez selectionner la requete 1, veuillez entrer une reference. \nPar exemple: 10 \n";
        sendMessage(socketClient, servAns1);

        break;

    case 2:
        //  servAns = "pas encore defini lol ";
        // write(socketClient, &servAns, 1200);
        // sendMessage(socketClient, servAns);
        char *servAns2 = (char *)malloc((9) * sizeof(char));
        servAns2 = "troisieme";
        sendMessage(socketClient, servAns2);
        break;

    case 3:
        //  servAns = "pas encore defini lol ";
        //  write(socketClient, &servAns, 1200);
        break;
    case 4:
        // servAns = "pas encore defini lol ";
        // write(socketClient, &servAns, 1200);
        break;
    default:
        break;
    }
}

*/


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
    // printf("message: %s \n", message);

    write(socketClient, &size, sizeof(int));
    // printf("%s \n", &message);

    write(socketClient, *&message, size);
}






// traitement des requete en fonction de clientAnswer
void queryTreatment(int clientAnswer, char *clientReference, int socketClient)
{

    switch (clientAnswer)
    {
    case 1:
        char line[1024];
        FILE *fp = fopen("bdd_bibliotheque.txt", "r");
        // read the open file
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            if (strstr(line, clientReference) != NULL)
            {
               
                    char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

                    strcpy(cRef, strtok(line, "#"));
                    strcpy(cAuthor, strtok(NULL, "#"));
                    strcpy(cTitle, strtok(NULL, "#"));
                    strcpy(cType, strtok(NULL, "#"));
                    strcpy(cNbPages, strtok(NULL, "#"));
                    strcpy(cRate, strtok(NULL, "#"));

                    printf("\n \n TATITAOU \n \n");

                    if (strcmp(cRef, clientReference) == 0)
                    {
                        printf("Reference: %s \n", cRef);
                        printf("Author: %s \n", cAuthor);
                        printf("Title: %s \n", cTitle);
                        printf("Type: %s \n", cType);
                        printf("NbPages: %s \n", cNbPages);
                        printf("Rate: %s \n", cRate);

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

                        // char * res = 'La reférence de votre livre est: ' + cRef + ' \nL auteur est: ' + cAuthor + ' \nLe titre est: ' + cTitle + ' \nLe type est: ' + cType + ' \nLe nombre de pages est: ' + cNbPages + ' \nLa note est: ' + cRate + ' \n';
                        sendMessage(socketClient, res);
                        //char *endOrNot = "Si vous n'avez plus de questions tapez 'oui' sinon tapez 'non'\n";
                       // sendMessage(socketClient, endOrNot); 
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

