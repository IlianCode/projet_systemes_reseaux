# projet_systemes_reseaux

Projet de L3 MIAGE à l'Université Grenoble Alpes consistant en la création d'une application client/serveur sur le modèle du protocole TCP/IP. L'objectif est de formuler des requêtes de livre via le client au serveur qui executera ces requêtes et cherchera dans la bibliothèque (base de données textuelle simpliste).
 <br>
 
 Une fois le projet téléchargé, il suffit d'ouvrir un terminal et de compiler le projet via la commande ```make``` .
 <br> 
 Deux éxécutables devraient alors être créés, le fichier "main" qui sera notre application serveur, ainsi que le fichier "client" qui sera notre application client. 
 <br>
 Ces deux fichiers seront créés dans le sous-répertoire /bin . <br>
 <br>
 Pour éxécuter le projet il suffira dans l'ordre de : <br>
 -Executer le serveur via la commande : ```./main XXX``` où XXX represente le numéro du port souhaité pour le serveur <br>
 -Executer le client via la commande : ```./client XXX YYYY``` où XXX est le nom du port du serveur et YYYY le nom du serveur. Si le serveur est en local, il s'agira de 'localhost'.<br>
 <br> 
 
 Ensuite il ne reste plus qu'à suivre les instructions fournies dans le terminal pour effectuer une recherche de livre.
 
