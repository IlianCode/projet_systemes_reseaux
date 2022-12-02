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
void traitementCaseTwo(char *myBookKey);
void traitementCaseThree(char *myBooks);
void traitementCaseFour(char *myBooks);

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
    if (typeQuery == 1)
    {

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
                        printf("toto");

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
        printf("toto2");

        //================================================================================================

        // reception de queryTreatment
        int size = 0;
        read(socketClient, &size, sizeof(int));
        char myBook[size];
        read(socketClient, &myBook, size);

        traitementCaseOne(myBook);

        // case 2:
        //    break;
    }
    else if (typeQuery == 2)
    {
        printf("\nVous avez selectionner la requete 2 !\n ");
        printf("Veuillez le nombre de mots clés que vous souhaitez (max 3).\n Tapez '1', '2' ou '3' ! \n");

        char checkNbKeyWord[2];
        int nbKeyWord = 0;

        // check if typequery is a number then send it to the server
        // do
        //{
        printf("Entrez le nombre de mots clés souhaité (1/2/3): \n");
        scanf(" %s", checkNbKeyWord);
        // } while (checkNbKeyWord != '1' && checkNbKeyWord != '2' && checkNbKeyWord != '3');

        nbKeyWord = atoi(checkNbKeyWord);
        char sendKeyWord[60];
        strcpy(sendKeyWord, checkNbKeyWord);
        strcat(sendKeyWord, "&");

        char keyWord1[20];
        char keyWord2[20];
        char keyWord3[20];

        switch (nbKeyWord)
        {

        case 1:
            printf("Veuillez entrer le mot clé 1.\n");
            scanf("%s", keyWord1);
            strcat(sendKeyWord, keyWord1);
            break;
        case 2:
            printf("Veuillez entrer le mot clé 1.\n");
            scanf("%s", keyWord1);

            printf("Veuillez entrer le mot clé 2.\n");
            scanf("%s", keyWord2);

            strcat(sendKeyWord, keyWord1);
            strcat(sendKeyWord, "&");
            strcat(sendKeyWord, keyWord2);
            break;
        case 3:
            printf("Veuillez entrer le mot clé 1.\n");
            scanf("%s", keyWord1);

            printf("Veuillez entrer le mot clé 2.\n");
            scanf("%s", keyWord2);

            printf("Veuillez entrer le mot clé 3.\n");
            scanf("%s", keyWord3);

            strcat(sendKeyWord, keyWord1);
            strcat(sendKeyWord, "&");
            strcat(sendKeyWord, keyWord2);
            strcat(sendKeyWord, "&");
            strcat(sendKeyWord, keyWord3);
            break;
        }
        printf("sendKeyWord : %s\n", sendKeyWord);
        sendMessage(socketClient, sendKeyWord);
        printf("bien envoyé\n");

        // recevoir le tableau de char
        int sizeBookKey = 0;
        read(socketClient, &sizeBookKey, sizeof(int));
        char myBookKey[sizeBookKey];
        read(socketClient, &myBookKey, sizeBookKey);

        printf("myBookKey : %s\n", myBookKey);

        traitementCaseTwo(myBookKey);
    }
    else if (typeQuery == 3)
    {
        printf("\nVous avez selectionner la requete 3 !\n ");
        printf("Veuillez entrer le nom de l'auteur. Par exemple: Victor Hugo \n");
        char author[50];
        scanf(" %[^\n]", author);

        printf("Veuillez entrer le genre recherché. Par exemple: roman \n");
        char *genre = (char *)malloc((50) * sizeof(char));
        scanf("%s", genre);

        // make a string with author and genre separated by a '&'
        char *authorAndGenre = (char *)malloc((100) * sizeof(char));
        strcpy(authorAndGenre, author);
        strcat(authorAndGenre, "&");
        strcat(authorAndGenre, genre);
        printf("authorAndGenre: %s\n", authorAndGenre);

        // send authorAndGenre to the server
        sendMessage(socketClient, authorAndGenre);
        printf("bien envoyé");

        // reception of the queryTreatment
        //  reception de queryTreatment
        int sizeBooks = 0;
        read(socketClient, &sizeBooks, sizeof(int));
        char myBooks[sizeBooks];
        read(socketClient, &myBooks, sizeBooks);

        printf("\n \n mysbooks : %s \n \n", myBooks);
        // traitementCaseOne(myBooks);

        traitementCaseThree(myBooks);

        //  case 4:
        //  break;
    }
    else if (typeQuery == 4)
    {
        printf("\nVous avez selectionner la requete 4 !\n ");
        printf("Veuillez entrer le nom de l'auteur. Par exemple: Victor Hugo \n");
        char author[50];
        scanf(" %[^\n]", author);

        printf("Voulez vous trier les livres:\n  -par nombre de pages? (croissant)\n -par note des lecteurs?\n");
        printf("Tapez 1 pour le nombre de pages, 2 pour la note des lecteurs\n");
        int choice;
        scanf("%d", &choice);
        // transform choice into a string
        char *choiceString = (char *)malloc((4) * sizeof(char));
        sprintf(choiceString, "%d", choice);
        // preparer la variable d'envoie
        char *myQuery = malloc(sizeof(char) * 54);
        strcpy(myQuery, author);
        strcat(myQuery, "&");
        strcat(myQuery, choiceString);
        // envoie
        sendMessage(socketClient, myQuery);

        // reception des livres en reponses
        int sizeBooksFour = 0;
        read(socketClient, &sizeBooksFour, sizeof(int));
        char myBooksFour[sizeBooksFour];
        read(socketClient, &myBooksFour, sizeBooksFour);

        printf("\n \n mysbooks : %s \n \n", myBooksFour);
        // traitementCaseFour(myBooksFour);

        traitementCaseFour(myBooksFour);
    }
}

bool askEnd(int socketClient)
{

    bool res = true;

    printf("Voulez vous faire une nouvelle recherche?\n Tapez 'non' pour sortir, sinon tapez 'oui' pour faire une nouvelle recherche ! \n");

    char answer[5];
    scanf(" %s", answer);
    if (strcmp(answer, "non") == 0)
    {
        res = false;
    }
    else
    {
        res = true;
    }
    return res;
}

void traitementCaseOne(char *myBook)
{

    char cNumber[4], cRef[20], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];
    strcpy(cNumber, strtok(myBook, "&"));
    if (cNumber == 0){
        printf("========================================\n");
        printf(" \nAucun livre ne correspond à votre recherche !\n\n");
        printf("========================================\n");
    }else {

    
    strcpy(cRef, strtok(NULL, "&"));
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

    if (atoi(cNbPages) > 300)
    {
        printf("\nLe nombre page du livre est superieur à 300 !!!! \n\n");
    }
    else
    {
        printf("\nLe nombre de pages inferieur à 300 !! \n ");
    }

    }
}

void displayClientMenu()
{
    printf("Quel type de requete voulez vous faire ?\n1.  Recherche par reference \n2.  Recherche par mot clé \n3.  Recherche par auteur et par genre littéraire\n4.  Recherche par auteur et par critère: nombre de pages ou note des lecteurs\n ");
}

void traitementCaseTwo(char *myBookKey)
{
    char cNumber[4];
    strcpy(cNumber, strtok(myBookKey, "&"));
    if (atoi(cNumber) == 0)
    {
        printf("========================================\n");
        printf(" \nAucun livre ne correspond à votre recherche !\n\n");
        printf("========================================\n");
    }
    else
    {

        printf("\n-------- Voici le(s) livre(s) trouvé en fonction de vos critères de recherche  --------\n");

        for (int i = 0; i < atoi(cNumber); i++)
        {
            char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[4];

            strcpy(cRef, strtok(NULL, "&"));
            strcpy(cAuthor, strtok(NULL, "&"));
            strcpy(cTitle, strtok(NULL, "&"));
            strcpy(cType, strtok(NULL, "&"));
            strcpy(cNbPages, strtok(NULL, "&"));
            strcpy(cRate, strtok(NULL, "&"));

            printf("-------------------------\n");
            printf("Livre %d : ", i + 1);

            printf("Reference: %s \n", cRef);
            printf("Auteur: %s \n", cAuthor);
            printf("Title: %s \n", cTitle);
            printf("Genre: %s \n", cType);
            printf("NbPages: %s \n", cNbPages);
            printf("Note: %s ", cRate);
            printf("-------------------------\n");
        }
        printf("-------------------------------------------------------------------\n");
    }
}

void traitementCaseThree(char *myBooks)
{
    char cNumber[10];
    strcpy(cNumber, strtok(myBooks, "&"));
    printf("cNumber : %s \n", cNumber);
    if (atoi(cNumber) == 0)
    {
        printf("========================================\n");
        printf(" \nAucun livre ne correspond à votre recherche !\n\n");
        printf("========================================\n");
    }
    else
    {
        printf("\n-------- Voici le(s) livre(s) trouvé en fonction de vos critères de recherche  --------\n");

        for (int i = 0; i < atoi(cNumber); i++)
        {
            char cRef[4], cTitle[50];

            strcpy(cTitle, strtok(NULL, "&"));
            strcpy(cRef, strtok(NULL, "&"));
            printf("-------------------------\n");
            printf("Livre %d : ", i + 1);
            printf("Title: %s \n", cTitle);
            printf("Reference: %s \n", cRef);
            printf("-------------------------\n");
        }

        printf("-------------------------------------------------------------------\n");
    }
}

void traitementCaseFour(char *myBooks)
{
    char cNumber[4];
    strcpy(cNumber, strtok(myBooks, "&"));

    if (atoi(cNumber) == 0)
    {
        printf("========================================\n");
        printf(" \nAucun livre ne correspond à votre recherche !\n\n");
        printf("========================================\n");
    }
    else
    {
    printf("\n-------- Voici le(s) livre(s) trouvé en fonction de vos critères de recherche  --------\n");

    for (int i = 0; i < atoi(cNumber); i++)
    {
        char cRef[4], cTitle[50], cAuthor[50], cType[50], cNbPages[50], cRate[4];

        strcpy(cRef, strtok(NULL, "&"));
        strcpy(cTitle, strtok(NULL, "&"));
        strcpy(cAuthor, strtok(NULL, "&"));
        strcpy(cType, strtok(NULL, "&"));
        strcpy(cNbPages, strtok(NULL, "&"));
        strcpy(cRate, strtok(NULL, "&"));

        printf("-------------------------\n");
        printf("Livre %d : \n", i + 1);
        printf("Reference: %s \n", cRef);
        printf("Title: %s \n", cTitle);
        printf("Auteur: %s \n", cAuthor);
        printf("Genre: %s \n", cType);
        printf("NbPages: %s \n", cNbPages);
        printf("Note: %s ", cRate);
        printf("-------------------------\n");
    }

    printf("-------------------------------------------------------------------\n");
}
}