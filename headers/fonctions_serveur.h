void end_of_child();
void createProcessClient(int socketClient, int socketServer);
void handleClient(int socketClient);
int searchSize(char *str);
void sendMessage(int socketClient, char *message);
void queryTreatment(int clientAnswer, int socketClient);
void researchOne(int socketClient);
void researchTwo(int socketCLient);
void researchThree(int socketClient);
void researchFour(int socketClient);
char *getNomAuteur(char *nomEntier);
int rateToInt(char *rate);
int compareString(char *str1, char *str2);
