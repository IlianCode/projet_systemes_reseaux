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


struct livre
{
    char reference[50];
    char auteur[50];
    char titre[50];
    char type[50];
    char nbPages[50];
    char rate[50];
};

int main(int argc, char *argv[])
{

    // Creation de la structure
    struct livre
    {
        char reference[50];
        char auteur[50];
        char titre[50];
        char type[50];
        char nbPages[50];
        char rate[50];
    };

    // mot clé recherché
    char *clavier = "roman";
    char line[1024];
    FILE *fp = fopen("bdd_bibliotheque.txt", "r");

    int compteur = 0;
    int size = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, clavier) != NULL)
        {
            size++;
        }
    }

    // fermeture du fichier
    fclose(fp);
    // réouverture du fichier
    fp = fopen("bdd_bibliotheque.txt", "r");
    printf("size : %d\n", size);
    
    // create list of struct livre of size number of livre avec mot clé
    struct livre tabLivre[size];
    // read the open file
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, clavier) != NULL)
        {

            char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

            strcpy(cRef, strtok(line, "#"));
            strcpy(cAuthor, strtok(NULL, "#"));
            strcpy(cTitle, strtok(NULL, "#"));
            strcpy(cType, strtok(NULL, "#"));
            strcpy(cNbPages, strtok(NULL, "#"));
            strcpy(cRate, strtok(NULL, "#"));

            strcpy(tabLivre[compteur].reference, cRef);
            strcpy(tabLivre[compteur].auteur, cAuthor);
            strcpy(tabLivre[compteur].titre, cTitle);
            strcpy(tabLivre[compteur].type, cType);
            strcpy(tabLivre[compteur].nbPages, cNbPages);
            strcpy(tabLivre[compteur].rate, cRate);

            compteur++;
        }
    }
    // fermeture du fichier
    fclose(fp);

    //print every tabLivre
    for (int i = 0; i < size; i++)
    {
        printf("reference : %s\n", tabLivre[i].reference);
        printf("auteur : %s\n", tabLivre[i].auteur);
        printf("titre : %s\n", tabLivre[i].titre);
        printf("type : %s\n", tabLivre[i].type);
        printf("nbPages : %s\n", tabLivre[i].nbPages);
        printf("rate : %s\n", tabLivre[i].rate);
    }
    //for each tabLivre if tabLIvre.auteur is alphabetically before tabLivre[i+1].auteur then swap
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (strcmp(tabLivre[i].auteur, tabLivre[j].auteur) < 0)
            {
                struct livre temp = tabLivre[i];
                tabLivre[i] = tabLivre[j];
                tabLivre[j] = temp;
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        printf("-----------------------\n");
        printf("reference : %s\n", tabLivre[i].reference);
        printf("auteur : %s\n", tabLivre[i].auteur);
        printf("titre : %s\n", tabLivre[i].titre);
        printf("type : %s\n", tabLivre[i].type);
        printf("nbPages : %s\n", tabLivre[i].nbPages);
        printf("rate : %s\n", tabLivre[i].rate);
    }
    
}
