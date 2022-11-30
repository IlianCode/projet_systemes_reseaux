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

int main(int argc, char *argv[]){


     char line[1024];
     char clientReference[4] = "70";

    char resAuthorType[600] = "Vos livres trouvé selon vos critères sont : \n";

    char author[12] = "Victor Hugo";

    int nbBooks = 0;
    FILE *fp = fopen("bdd_bibliotheque.txt", "r");
        //get number of books form the database
        while (fgets(line, sizeof(line), fp) != NULL){
            nbBooks++;
        }
        //create the array of books
        char books[nbBooks][6][50];
        int i = 0;
        int j = 0;
        
        
        // read the open file
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            if (strstr(line, author) != NULL)
            {
                
               
                    char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

                    strcpy(cRef, strtok(line, "#"));
                    strcpy(cAuthor, strtok(NULL, "#"));
                    strcpy(cTitle, strtok(NULL, "#"));
                    strcpy(cType, strtok(NULL, "#"));
                    strcpy(cNbPages, strtok(NULL, "#"));
                    strcpy(cRate, strtok(NULL, "#"));

                   // printf("\n \n TATITAOU \n \n");

                    if (strcmp(author, cAuthor) == 0)
                    {
                        printf("Reference: %s \n", cRef);
                        strcpy(books[i][0] , cRef);
                        strcpy(books[i][1] , cAuthor);
                        strcpy(books[i][2] , cTitle);
                        strcpy(books[i][3] , cType);
                        strcpy(books[i][4] , cNbPages);
                        strcpy(books[i][5] , cRate);

                        //printf("Author: %s \n", cAuthor);
                        printf("Title: %s \n", cTitle);
                       // printf("Type: %s \n", cType);
                       // printf("NbPages: %s \n", cNbPages);
                        //printf("Rate: %s \n", cRate);

                        // char * res = 'La reférence de votre livre est: ' + cRef + ' \nL auteur est: ' + cAuthor + ' \nLe titre est: ' + cTitle + ' \nLe type est: ' + cType + ' \nLe nombre de pages est: ' + cNbPages + ' \nLa note est: ' + cRate + ' \n';
                        //sendMessage(socketClient, res);
                        //char *endOrNot = "Si vous n'avez plus de questions tapez 'oui' sinon tapez 'non'\n";
                        //sendMessage(socketClient, endOrNot); 
                        //break;

                        char resTitle[50] = "Le titre est: ";
                        strcat(resAuthorType, cTitle);
                        strcat(resAuthorType, "\n");
                        char resReference[50] = "La référence est: ";
                        strcat(resAuthorType, cRef);
                        strcat(resAuthorType, "\n");

                        i++;
                    }
                    /*else
                    {
                        char *res = "Aucune référence pour votre livre n'est trouvé.";

                        sendMessage(socketClient, res);
                    }*/
                
            }
        }
        printf("%s", resAuthorType);
}