#include "../headers/fonctions_serveur.h"
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
//fonction appeler dans le handler pour gerer les processus zombies
void end_of_child(){
    wait(NULL);
}

//fonction créant un fork pour gérer les clients a chaque connection
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
        printf("Client disconnected successfully\n");
        exit(0);
    default:
        close(socketClient);
    }
}

//fonction gérant les clients
void handleClient(int socketClient)
{
    int endClient = 0;
    printf("Client connected\n");

    while (endClient != 1)
    {

        // receive the query type of the client
        int clientAnswer = 0;
        read(socketClient, &clientAnswer, sizeof(int));
        
        //selon le type de requete on appelle la fonction correspondante dans queryTreatment
        queryTreatment(clientAnswer, socketClient);

        // lis la reponse du client
        read(socketClient, &endClient, sizeof(int));
    }

    // close the socket
    close(socketClient);
}



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

// traitement des requete en fonction de clientAnswer
void queryTreatment(int clientAnswer, int socketClient)
{

    switch (clientAnswer)
    {
    case 1:
        researchOne(socketClient);
        break;
    case 2:
        researchTwo(socketClient);

        break;
    case 3:
        researchThree(socketClient);
        break;
    case 4:
        researchFour(socketClient);
        break;
    }
}

// recherche par référence, numero 1 dans l'enoncé
void researchOne(int socketClient)
{
    // lecture reference client int :
    int clientReference = 0;
    read(socketClient, &clientReference, sizeof(int));

    // transform clientReference to string to search in the file
    char clientReferenceString[4];
    sprintf(clientReferenceString, "%d", clientReference);
    char line[1024];
    FILE *fp = fopen("../sources/bdd_bibliotheque.txt", "r");
    int count = 0;
    char countString[4];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        //si clientReferenceString est dans la ligne on envoie la ligne au client 
        if (strstr(line, clientReferenceString) != NULL)
        {

            char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

            strcpy(cRef, strtok(line, "#"));
            strcpy(cAuthor, strtok(NULL, "#"));
            strcpy(cTitle, strtok(NULL, "#"));
            strcpy(cType, strtok(NULL, "#"));
            strcpy(cNbPages, strtok(NULL, "#"));
            strcpy(cRate, strtok(NULL, "#"));

            if (atoi(cRef) == clientReference)
            {
                count++;
                sprintf(countString, "%d", count);


                char res[560];
                strcpy(res, countString);
                strcat(res, "&");
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


                sendMessage(socketClient, res);
                memset(res, 0, strlen(res));

                break;
            }
            
        }
    }
    char ifNoRes[4];
    //sinon envoie un message d'erreur au client
    if(count == 0){
        strcpy(ifNoRes, "0&");
        sendMessage(socketClient, ifNoRes);
    }
}

// recherche par mot clé et trie alphabetique, numero 2 dans l'enoncé
void researchTwo(int socketCLient)
{
    struct livre
    {
        char reference[50];
        char auteur[50];
        char titre[50];
        char type[50];
        char nbPages[50];
        char rate[50];
        char fullAuteur[50];
    };

    // reception du ou des mots clés
    int size = 0;
    read(socketCLient, &size, sizeof(int));
    char *clientKeywords = malloc(size);
    read(socketCLient, clientKeywords, size);

    char cNbKeyWord[4], cKeyWord1[20], cKeyWord2[20], cKeyWord3[20];

    strcpy(cNbKeyWord, strtok(clientKeywords, "&"));
    //les mots clés sont initilisé avec un charactere non présent dans le fichier 
    //afin de ne pas avoir de problème si le client ne rentre pas 3 mots clés
    char *clavier = "}";
    char *clavier2 = "}";
    char *clavier3 = "}";

    //selon le nombre de mots clés souhaité, les variables clavier prennent la valeur des mots clé saisis par le client
    switch (atoi(cNbKeyWord))
    {
    case 1:
        strcpy(cKeyWord1, strtok(NULL, "&"));
        clavier = cKeyWord1;

        break;
    case 2:
        strcpy(cKeyWord1, strtok(NULL, "&"));
        strcpy(cKeyWord2, strtok(NULL, "&"));

        clavier = cKeyWord1;
        clavier2 = cKeyWord2;
        break;
    case 3:
        strcpy(cKeyWord1, strtok(NULL, "&"));
        strcpy(cKeyWord2, strtok(NULL, "&"));
        strcpy(cKeyWord3, strtok(NULL, "&"));
   
        clavier = cKeyWord1;
        clavier2 = cKeyWord2;
        clavier3 = cKeyWord3;
        break;
    }

//debut de trie
    char line[1024];
    FILE *fp = fopen("bdd_bibliotheque.txt", "r");
    int compteur = 0;
    int sizeResearchTwo = 0;
    //regarde le nombre d'occurrences dans le fichier pour les mots clés saisis par le client
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if ((strstr(line, clavier) != NULL) || (strstr(line, clavier2) != NULL) || (strstr(line, clavier3) != NULL))
        {
            sizeResearchTwo++;
        }
    }

    // fermeture du fichier
    fclose(fp);
    // réouverture du fichier
    fp = fopen("bdd_bibliotheque.txt", "r");


    // create list of struct livre of size number of livre avec mot clé
    struct livre tabLivre[sizeResearchTwo];
    // read the open file

//remplissage de la liste de struct livre
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if ((strstr(line, clavier) != NULL) || (strstr(line, clavier2) != NULL) || (strstr(line, clavier3) != NULL))
        {
            char cRef[4], cAuthor[50], cTitle[50], cType[50], cNbPages[50], cRate[50];

            strcpy(cRef, strtok(line, "#"));
            strcpy(cAuthor, strtok(NULL, "#"));
            strcpy(cTitle, strtok(NULL, "#"));
            strcpy(cType, strtok(NULL, "#"));
            strcpy(cNbPages, strtok(NULL, "#"));
            strcpy(cRate, strtok(NULL, "#"));

            strcpy(tabLivre[compteur].reference, cRef);
            strcpy(tabLivre[compteur].auteur, getNomAuteur(cAuthor));
            strcpy(tabLivre[compteur].titre, cTitle);
            strcpy(tabLivre[compteur].type, cType);
            strcpy(tabLivre[compteur].nbPages, cNbPages);
            strcpy(tabLivre[compteur].rate, cRate);
            strcpy(tabLivre[compteur].fullAuteur, cAuthor);

            compteur++;
        }
    }
    // fermeture du fichier
    fclose(fp);

    // for each tabLivre if tabLIvre.auteur is alphabetically before tabLivre[i+1].auteur then swap
    for (int i = 0; i < sizeResearchTwo; i++)
    {
        for (int j = 0; j < sizeResearchTwo; j++)
        {
            // strtok to get the last word of the string
            // compare the last word of the string
            if (strcmp(tabLivre[i].auteur, tabLivre[j].auteur) < 0)
            {
                struct livre temp = tabLivre[i];
                tabLivre[i] = tabLivre[j];
                tabLivre[j] = temp;
            }
        }
    }

    char res[2048]; 
    // create a variable to store the sizeResearchTwo in string
    char cSizeResearchTwo[10];
    // convert sizeResearchTwo to string
    sprintf(cSizeResearchTwo, "%d", sizeResearchTwo);
    
    //ecriture du message qui sera renvoyé au client
    for (int i = 0; i < sizeResearchTwo; i++)
    { 
        if (i == 0)
        {
            strcpy(res, cSizeResearchTwo);
            strcat(res, "&");
        }
        else
        {
            strcat(res, "&");
        }

        strcat(res, tabLivre[i].reference);
        strcat(res, "&");

        if (strcmp(tabLivre[i].fullAuteur, tabLivre[i].auteur) != 0)
        {
            if (strcmp(tabLivre[i].fullAuteur, "Honore") == 0 && strcmp(tabLivre[i].auteur, "Balzac") == 0)
            {
                strcat(res, "Honore de Balzac");
            }
            else
            {
                strcat(res, tabLivre[i].fullAuteur);
                strcat(res, " ");
                strcat(res, tabLivre[i].auteur);
            }
        }
        else
        {
            strcat(res, tabLivre[i].auteur);
        }
        strcat(res, "&");

        strcat(res, tabLivre[i].titre);
        strcat(res, "&");
        strcat(res, tabLivre[i].type);
        strcat(res, "&");
        strcat(res, tabLivre[i].nbPages);
        strcat(res, "&");
        strcat(res, tabLivre[i].rate);
    }

// si pas de livre trouvé on envoie un message d'erreur au client
    if (sizeResearchTwo == 0)
    {
        strcpy(res, "0&");
    }
    // send the result to the client
    sendMessage(socketCLient, res);
}

// recherche par auteur puis par genre , numero 3 dans l'enoncé
void researchThree(int socketClient)
{
    // reception authorAndGenre du client
    // read size of authorAndGenre
    int size = 0;
    read(socketClient, &size, sizeof(int));
    // read authorAndGenre
    char authorAndGenre[size];
    read(socketClient, &authorAndGenre, size);

    // split authorAndGenre in two string
    char *author = strtok(authorAndGenre, "&");
    char *genre = strtok(NULL, "&");

    char line[1024];
    char resTitle[500];

    int count = 0;

    FILE *fp = fopen("bdd_bibliotheque.txt", "r");
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

            if (strcmp(genre, cType) == 0)
            {


                strcat(resTitle, "&");

                strcat(resTitle, cTitle);
                strcat(resTitle, "&");
                strcat(resTitle, cRef);
                count++;
            }
            /*else
            {
                char *res = "Aucune référence pour votre livre n'est trouvé.";

                sendMessage(socketClient, res);
            }*/
        }
    }
   
    char newCount[4];
    //si pas de livre trouvé on envoie un message d'erreur au client
    if (count == 0)
    {
        strcpy(resTitle, "0&");
        sendMessage(socketClient, resTitle);
       
    }
    else //sinon ajout du nombre d'occurrence devant le message
    {

        sprintf(newCount, "%d", count);
        strcat(newCount, resTitle);
        sendMessage(socketClient, newCount);
    }
}

//recherche par auteur puis trie par nb page ou note 
void researchFour(int socketClient)
{
    // reception autheur et choix du client
    //  read size of authorAndChoice
    int size = 0;
    read(socketClient, &size, sizeof(int));
    // read authorAndChoice
    char authorAndChoice[size];
    read(socketClient, &authorAndChoice, size);

   

    // split authorAndChoice in two string
    char *author = strtok(authorAndChoice, "&");
    char *choice = strtok(NULL, "&");
    
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
    char *clavier = author;
    char line[1024];
    FILE *fp = fopen("bdd_bibliotheque.txt", "r");
    int choixTrie = atoi(choice);
    int compteur = 0;
    int sizeFour = 0;
    //calcul du nombre d'occurrences
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, clavier) != NULL)
        {
            sizeFour++;
        }
    }

    // fermeture du fichier
    fclose(fp);
    // réouverture du fichier
    fp = fopen("bdd_bibliotheque.txt", "r");


    // create list of struct livre of size number of livre avec mot clé
    struct livre tabLivre[sizeFour];
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
    // fermeture du fichier
    fclose(fp);

    // print every tabLivre
   


    //si choix trie par nb page
    if (choixTrie == 1)
    {
        // sort tabLivre by nbPages (bubble sort)
        for (int i = 0; i < sizeFour; i++)
        {
            for (int j = 0; j < sizeFour - 1; j++)
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
    //sinon si choix par note 
    else if (choixTrie == 2)
    {

        // sort tabLivre by rate (bubble sort)

        for (int i = 0; i < sizeFour; i++)
        {
            for (int j = 0; j < sizeFour - 1; j++)
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

   //ecriture du message à envoyer au client
    char resSearchFour[1024];
    // transform sizeFour to string
    char sizeFourString[4];
    sprintf(sizeFourString, "%d", sizeFour);

    for (int i = 0; i < sizeFour; i++)
    {

        // prepare message to send
        if (i == 0)
        {
            strcpy(resSearchFour, sizeFourString);
        }
        strcat(resSearchFour, "&");
        strcat(resSearchFour, tabLivre[i].reference);
        strcat(resSearchFour, "&");
        strcat(resSearchFour, tabLivre[i].auteur);
        strcat(resSearchFour, "&");
        strcat(resSearchFour, tabLivre[i].titre);
        strcat(resSearchFour, "&");
        strcat(resSearchFour, tabLivre[i].type);
        strcat(resSearchFour, "&");
        strcat(resSearchFour, tabLivre[i].nbPages);
        strcat(resSearchFour, "&");
        strcat(resSearchFour, tabLivre[i].rate);
    }//si pas de livre trouvé envoie d'un message d'erreur au client
    if (sizeFour == 0)
    {
        strcpy(resSearchFour, "&0");
    }
   

    // send message to client
    sendMessage(socketClient, resSearchFour);
}

//fonction pour trouver le dernier mot du nom d'un auteur, pour le trie par ordre alphabetique
char *getNomAuteur(char *nomEntier)
{
    
    // get the last word of the string nomEntier
    char *nomAuteur = strtok(nomEntier, " ");
    char *temp = nomAuteur;
    while (temp != NULL)
    {
        
        nomAuteur = temp;
        temp = strtok(NULL, " ");
        
    }
    return nomAuteur;
}

//changer une note A B C ou D en chiffre 4 3 2 ou 1 afin de pouvoie appliquer un trie a bulle dessus
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

//fonction pour comparer deux string , necessaire pour la fonction rateToInt
int compareString(char *str1, char *str2)
{
    int res = 0;
    int i = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            res = 1;
        }
        i++;
    }
    
    return res;
}