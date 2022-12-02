#include <stdbool.h>
int searchSize(char *str);
void sendMessage(int socketClient, char *message);
void handleClientQuery(int socketClient, int typeQuery);
bool askEnd(int socketClient);
void traitementCaseOne(char *myBook);
void displayClientMenu();
void traitementCaseTwo(char *myBookKey);
void traitementCaseThree(char *myBooks);
void traitementCaseFour(char *myBooks);
