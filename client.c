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

void displayClientMenu();
void handleClientQuery(int socketClient, int typeQuery);
bool askEnd(int socketClient);
void traitementCaseOne(char *myBook);

int main(int argc, char *argv[])
{
    // create the client part of the tcpi ip server
    struct sockaddr_in addrClient;
    int socketClient;
    const char *namehost = "localhost";
    // def the structure of the server
    struct hostent *infos_server = NULL;

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == -1)
    {
        printf("Error creating socket");
        exit(-1);
    }

    infos_server = gethostbyname(namehost);
    // check if infos_server is still null
    if (infos_server == NULL)
    {
        printf("Error getting host by name");
        exit(-1);
    }

    // give value to the addrClient
    addrClient.sin_family = AF_INET;
    memcpy(&addrClient.sin_addr.s_addr, infos_server->h_addr, sizeof(u_long));
    addrClient.sin_port = htons(9999);

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
        // test reception menu
        displayClientMenu();
        

        // envoie de la reponse 1/2/3/4
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

       
        //=============================

        handleClientQuery(socketClient, typeQuery);

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

// function

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
    write(socketClient, &size, sizeof(int));
    write(socketClient, *&message, size);
}

void handleClientQuery(int socketClient, int typeQuery)
{
    switch (typeQuery)
    {
    case 1:
        printf("\nVous avez selectionner la requete 1 !\n ");
        
        // envoie de la reference envoyé par le client
        char *checkReference = (char *)malloc((10) * sizeof(char));
        int reference = 0;
        bool isNumber = false;
        // ask to input a int in checkreference and check if it's a number
        // do it until its a number
        while (isNumber == false)
        {
            printf("Veuillez entrer une reference. Par exemple: 10 \n");
            scanf("%s", checkReference);

            for (int i = 0; i < strlen(checkReference); i++)
            {
                if (isdigit(checkReference[i]))
                {
                    isNumber = true;
                }
                else
                {
                    isNumber = false;
                    printf("\npas un nombre\n");
                    break;
                }
            }
        }

        reference = atoi(checkReference);
        write(socketClient, &reference, sizeof(int));

    


        //================================================================================================

        // reception de queryTreatment
        int size = 0;
        read(socketClient, &size, sizeof(int));
        char myBook[size];
        read(socketClient, &myBook, size);

        traitementCaseOne(myBook);
        break;

        /*case 2:
            break;
        case 3:
            break;
        case 4:
            break;*/
    }
}

bool askEnd(int socketClient)
{

    bool res = true;
    
    printf("Voulez vous faire une nouvelle recherche?\n Tapez 'non' pour sortir, sinon tapez 'oui' pour faire une nouvelle recherche ! \n");

    char *answer;
    scanf("%s", answer);
    if (strcmp(answer, "non") == 0)
    {
        res = false;
    }else{
        res = true;
    }
    return res;
}

void traitementCaseOne(char *myBook)
{
    char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

    strcpy(cRef, strtok(myBook, "&"));
    strcpy(cAuthor, strtok(NULL, "&"));
    strcpy(cTitle, strtok(NULL, "&"));
    strcpy(cType, strtok(NULL, "&"));
    strcpy(cNbPages, strtok(NULL, "&"));
    strcpy(cRate, strtok(NULL, "&"));

    printf("\n-------- voici les informations sur le livre de reference %s --------\n", cRef);
    printf("Reference: %s \n", cRef);
    printf("Author: %s \n", cAuthor);
    printf("Title: %s \n", cTitle);
    printf("Type: %s \n", cType);
    printf("NbPages: %s \n", cNbPages);
    printf("Rate: %s ", cRate);
    printf("-------------------------------------------------------------------\n");
    atoi(cRef);

    if (atoi(cNbPages) > 300)
    {
        printf("\nLe nombre page du livre est superieur à 300 !!!! \n\n");
    }
    else
    {
        printf("\nLe nombre de pages inferieur à 300 !! \n ");
    }
}

void displayClientMenu()
{
    printf("Quel type de requete voulez vous faire ?\n1.  Recherche par reference \n2.  Recherche par mot clé \n3.  Recherche par auteur et par genre littéraire\n4.  Recherche par auteur et par critère: nombre de pages ou note des lecteurs\n ");
}
