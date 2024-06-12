#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAXMSG MAXREP

int main() {
    char reponse[MAXREP]; // pour stocker la réponse du serveur


    // Affiche les échanges avec le serveur (false pour désactiver)
    mode_debug(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr", 9999);
    // utilisez le port 443 en cas de problème sur le 9999
    /* connexion("im2ag-appolab.u-ga.fr", 443); */

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI", reponse);
    envoyer_recevoir("load tutorielFORT", reponse);
    envoyer_recevoir("start", reponse);
    envoyer_recevoir("OK", reponse);
    envoyer_recevoir("OUI", reponse);
    int j = 0;
    while (j <= 10)
    {
        // Boucle interne pour mettre en majuscules et afficher les réponses du serveur
        char chaine[MAXREP];
        int i = 0;
        
        while (reponse[i] != '\0') {
            chaine[i] = toupper(reponse[i]);
            i++;
        }
        chaine[i] = '\0';

        // Notez qu'ici on n'utilise pas la reponse du serveur
        envoyer_recevoir(chaine, reponse);
        printf("Réponse du serveur: %s", reponse);
        j++;
    }
    printf("Fin de la connection au serveur\n");
    return 0;
}
