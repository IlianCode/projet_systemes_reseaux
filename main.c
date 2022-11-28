#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>



int main ( int argc , char * argv[]){
    int socketServer, socketClient;
    struct sockaddr_in adresse;
    int binded;

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

    //start accepting connections
    socketClient = accept(socketServer, NULL, NULL);

    while(1){
        //do something
    }
    return 0;


}
   