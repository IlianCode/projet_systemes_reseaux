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

void displayClientMenu();
void handleClientQuery(int socketClient, int typeQuery);
bool askEnd(int socketClient);

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
    //debut reception données et envoie de reponse
    while (isRunning)
    {
        //test reception menu 
        char messageRecu[282];
        read(socketClient, &messageRecu, 282);
        printf("%s", messageRecu);


        //char *str = "Hello world";
        //write(socketClient, &str, strlen(str));
        int typeQuery = 0;
            scanf("%d", &typeQuery);
            write(socketClient, &typeQuery, sizeof(int));

        
    
        //reception de sendResponseToQuery
        
        int size = 0;
        read(socketClient, &size, sizeof(int));
        printf("size = %d \n", size);

        char msgAns[size];
        read(socketClient, msgAns, size);
        printf("%s \n", msgAns);

        handleClientQuery(socketClient, typeQuery);
        
        isRunning=askEnd(socketClient);

        //envoie 0 au serveur si isRunning = true sinon envoie 1 au serveur
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


int searchSize(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i+1;
}

void sendMessage (int socketClient, char *message)
{
    int size = searchSize(message)*sizeof(char);
    write(socketClient, &size, sizeof(int));
    write(socketClient, *&message, size);
}

void handleClientQuery(int socketClient, int typeQuery){
    switch (typeQuery)
    {
    case 1:
        char * reference = (char*) malloc((4)*sizeof(char));
        
        scanf("%s", reference);
    
        sendMessage(socketClient, reference);

        //reception de queryTreatment
        int size = 0;
        read(socketClient, &size, sizeof(int));
        printf("size = %d \n", size);
        char myBook[size];
        read(socketClient, &myBook, size);     
        printf("%s \n", myBook);



        break;


    /*case 2:
        break;
    case 3:
        break;
    case 4:
        break;*/
    }
     
}

bool askEnd(int socketClient){

        bool res = true;
        //reception du message de fin de traitement
        int size = 0;
        read(socketClient, &size, sizeof(int));
        printf("size = %d \n", size);
        char endTreatment[size];
        read(socketClient, &endTreatment, size);     
        printf("%s \n", endTreatment);

        char *answer;
        scanf("%s", answer);
        if(strcmp(answer, "non") == 0){
            res = false;
        }  
        return res;
        
}