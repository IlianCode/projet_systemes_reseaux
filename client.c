#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>

int main ( int argc , char * argv[]){
    //create the client part of the tcpi ip server
    struct sockaddr_in addrClient;
    int socketClient;
    const char * namehost = "localhost";
    //def the structure of the server
    struct hostent * infos_server= NULL;

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == -1){
        printf("Error creating socket");
        exit(-1);
    } 
    

    infos_server = gethostbyname(namehost);
//check if infos_server is still null
    if(infos_server == NULL){
        printf("Error getting host by name");
        exit(-1);
    } 

    //give value to the addrClient 
    addrClient.sin_family = AF_INET;
    memcpy(&addrClient.sin_addr.s_addr, infos_server->h_addr , sizeof(u_long));
    addrClient.sin_port = htons(9999);

    //connect to the server
    if(connect(socketClient, (struct sockaddr *)&addrClient, sizeof(addrClient)) == -1){
        printf("Error connecting to server");
        exit(-1);
    } else {
        printf("Connected to server \n");
    }

    //send a message to the server
    int message = 2580;
    write(socketClient, &message, sizeof(int));
    int reponse= 0;
    read(socketClient, &reponse, sizeof(int));
    printf("Message received from server: %d \n", reponse);

    close(socketClient);
    return 0;


}