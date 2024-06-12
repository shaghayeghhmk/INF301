#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "client.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXMSG MAXREP

void decryptCesar(char *texteChiffre) {
    int i;
    int longueur = strlen(texteChiffre);

    for (i = 0; i < longueur; i++) {
        char caractere = texteChiffre[i];

        if (caractere >= 'A' && caractere <= 'Z') {
            // Décalage pour les lettres majuscules
            caractere = ((caractere - 'A' - 5 + 26) % 26) + 'A';
        } else if (caractere >= 'a' && caractere <= 'z') {
            // Décalage pour les lettres minuscules
            caractere = ((caractere - 'a' - 5 + 26) % 26) + 'a';
        }

        printf("%c", caractere);
    }
    printf("\n");
}



int main() {
    char reponse[MAXREP];
    // Affiche les échanges avec le serveur (false pour désactiver)
    mode_debug(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI",reponse);
    envoyer_recevoir("load projetX",reponse);
    envoyer_recevoir("help",reponse);
    envoyer_recevoir("start",reponse);
    envoyer_recevoir("veni vidi vici",reponse);
    printf ("Fin d'envoi des messages.\n");
    deconnexion();
    printf ("Fin de la connection au serveur\n");
    return 0;
}