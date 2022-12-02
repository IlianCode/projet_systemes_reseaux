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

int rateToInt(char *rate);
int compareString(char *str1, char *str2);

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
        char rate[3];
        char fullAuteur[50];
    };

    // mot clé recherché
    char *clavier = "Victor";
    char line[1024];
    FILE *fp = fopen("bdd_bibliotheque.txt", "r");
    int choixTrie = 1;
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
        if ((strstr(line, clavier) != NULL))
        {
            char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50];

            char cRate[3];

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
            strcpy(tabLivre[compteur].fullAuteur, cAuthor);

            compteur++;
        }
    }
    printf("sortie\n");
    // fermeture du fichier
    fclose(fp);

    // print every tabLivre
    for (int i = 0; i < size; i++)
    {
        printf("reference : %s\n", tabLivre[i].reference);
        printf("auteur : %s\n", tabLivre[i].auteur);
        printf("titre : %s\n", tabLivre[i].titre);
        printf("type : %s\n", tabLivre[i].type);
        printf("nbPages : %s\n", tabLivre[i].nbPages);
        printf("rate : %s\n", tabLivre[i].rate);
        printf("fullAuteur : %s %s\n", tabLivre[i].fullAuteur, tabLivre[i].auteur);
    }
    if (choixTrie == 1)
    {

        // sort tabLivre by nbPages (bubble sort)
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - 1; j++)
            {
                if (atoi(tabLivre[j].nbPages) > atoi(tabLivre[j + 1].nbPages))
                {
                    struct livre temp = tabLivre[j];
                    tabLivre[j] = tabLivre[j + 1];
                    tabLivre[j + 1] = temp;
                }
            }
        }
        
    }
    else if (choixTrie == 2)
    {

        // sort tabLivre by rate (bubble sort)

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - 1; j++)
            {

                if (rateToInt(tabLivre[j].rate) < rateToInt(tabLivre[j + 1].rate))
                {
                    struct livre temp = tabLivre[j];
                    tabLivre[j] = tabLivre[j + 1];
                    tabLivre[j + 1] = temp;
                }
            }
        }

        
    }

    // affichage du tableau trié
    printf("====================================");
    for (int i = 0; i < size; i++)
    {
        printf("reference : %s\n", tabLivre[i].reference);
        printf("auteur : %s\n", tabLivre[i].auteur);
        printf("titre : %s\n", tabLivre[i].titre);
        printf("type : %s\n", tabLivre[i].type);
        printf("nbPages : %s\n", tabLivre[i].nbPages);
        printf("rate : %s\n", tabLivre[i].rate);
        printf("fullAuteur : %s %s\n", tabLivre[i].fullAuteur, tabLivre[i].auteur);
    }
    printf(" rate to int = %d\n ", rateToInt(tabLivre[0].rate));

    return 0;
}

int rateToInt(char *rate)
{
    // compare rate to A B C and D then return 4 3 2 or 1
    char rateA[3] = "A";
    char rateB[3] = "B";
    char rateC[3] = "C";
    char rateD[3] = "D";
    int res = 0;

    if (compareString(rateA, rate) == 0)
    {
        res = 4;
    }
    else if (compareString(rateB, rate) == 0)
    {
        res = 3;
    }
    else if (compareString(rateC, rate) == 0)
    {
        res = 2;
    }
    else if (compareString(rateD, rate) == 0)
    {
        res = 1;
    }
    return res;
}

int compareString(char *str1, char *str2)
{
    int res = 0;
    int i = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            // printf("str1[%d] != str2[%d]\n", i, i);
            res = 1;
        }
        i++;
    }
    printf("res = %d\n", res);
    return res;
}