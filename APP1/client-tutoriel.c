#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAXMSG MAXREP

int main() {
    char reponse[MAXREP]; // pour stocker la réponse du serveur
    char message[MAXMSG]; // pour stocker le message à envoyer au serveur

    // Affiche les échanges avec le serveur (false pour désactiver)
    mode_debug(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr", 9999);
    // utilisez le port 443 en cas de problème sur le 9999
    /* connexion("im2ag-appolab.u-ga.fr", 443); */

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI", reponse);
    envoyer_recevoir("load tutoriel", reponse);
    envoyer_recevoir("start", reponse);
    envoyer_recevoir("oui", reponse);
    envoyer_recevoir("4", reponse);
    envoyer_recevoir("blanc", reponse);
    envoyer_recevoir("pincemoi", reponse);
    envoyer_recevoir("tutoriel", reponse);
    // Notez qu'ici on n'utilise pas la reponse du serveur

    lire_clavier(message);   // message tapé au clavier
    envoyer_recevoir(message, reponse); // on envoie message, et on reçoit la réponse du serveur

    printf ("Réponse du serveur: %s", reponse);

    printf ("Fin de la connection au serveur\n");
    return 0;
}
