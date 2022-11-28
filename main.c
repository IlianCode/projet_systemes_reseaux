#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>

void createProcessClient(int socketClient, int socketServer);
void handleClient(int socketClient);


int main ( int argc , char * argv[]){
    int socketServer, socketClient;
    struct sockaddr_in adresse;
    int binded;
    int clientAddresseLen;

    //create a tcpip server socket
    //create a listening socket
    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(9999);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);
   

   //check if socker server is running
    if (socketServer == -1){
        printf("Error creating socket");
        exit(1);
    } else {
        printf("Socket created successfully");
    }

    //attach the listening socket to an address
    binded = bind(socketServer, (struct sockaddr *)&adresse, sizeof(adresse));
    if (binded == -1){
        printf("Error binding socket");
        exit(1);
    } else {
        printf("Socket binded successfully");
    }

    //start listenning on the socket
    if(listen(socketServer, 5) == -1){
        printf("Error listening on socket");
        exit(1);
    } else {
        printf("Socket listening successfully");
    }
    clientAddresseLen = sizeof(adresse);

    while(1){
        //listening for query from client
        socketClient = accept(socketServer, (struct sockaddr *)&adresse, &clientAddresseLen);
        if (socketClient == -1){
            printf("Error accepting client");
            exit(1);
        }
        //if a connexion is established call the function createProcessClient
        if (socketClient != -1){
            createProcessClient(socketClient, socketServer);
        }
    return 0;
}
}   
void createProcessClient(int socketClient, int socketServer){
    switch (fork()){
        case -1:
            printf("Error creating process");
            exit(1);
        case 0:
     else {
            printf("Client accepted successfully");
        }           exit(0);
        default:
            close(socketClient);
    }
}

//function to listen to the client query
void handleClient(int socketClient){
    printf("Client connected");

    //send a "10" to the client to confirm the connexion
    send(socketClient, "10", 1, 0);
    //close the socket
    close(socketClient);

}
