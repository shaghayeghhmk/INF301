#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAXMSG MAXREP

int main() {
    char reponse[MAXREP]; 
    char message[MAXMSG]; 
    mode_debug(true);
    connexion("im2ag-appolab.u-ga.fr", 9999);
    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI", reponse);
    envoyer_recevoir("load planB", reponse);
    envoyer_recevoir("help", reponse);

    // Affiche les échanges avec le serveur
    char *premiereLigne = strtok(reponse, "\n");
    printf("Première Ligne de la Réponse d'Aide : %s\n", premiereLigne);

    // Trouve la valeur de décalage pour 'Chere Alice'
    int valeurDecalage = 0;
    char premiereLigneDechiffree[100]; // Ajustez la taille si nécessaire
    for (int decalage = 0; decalage < 26; decalage++) {
        for (int i = 0; i < strlen(premiereLigne); i++) {
            char caractere = premiereLigne[i];
            if (isalpha(caractere)) {
                char base = islower(caractere) ? 'a' : 'A';
                premiereLigneDechiffree[i] = ((caractere - base - decalage + 26) % 26) + base;
            } else {
                premiereLigneDechiffree[i] = caractere; // Les caractères non alphabétiques restent inchangés
            }
        }
        premiereLigneDechiffree[strlen(premiereLigne)] = '\0';
        
        if (strstr(premiereLigneDechiffree, "Chere Alice") != NULL) {
            valeurDecalage = decalage;
            break;
        }
    }
    printf("Valeur de Décalage pour 'Chere Alice' : %d\n", valeurDecalage);

    // Déchiffre "hasta la revolucion" avec la valeur négative du décalage
    char messageDecode[100]; // Ajustez la taille si nécessaire
    for (int i = 0; i < strlen("hasta la revolucion"); i++) {
        char caractere = "hasta la revolucion"[i];
        if (isalpha(caractere)) {
            char base = islower(caractere) ? 'a' : 'A';
            messageDecode[i] = ((caractere - base + (26 - valeurDecalage)) % 26) + base;
        } else {
            messageDecode[i] = caractere; // Les caractères non alphabétiques restent inchangés
        }
    }
    messageDecode[strlen("hasta la revolucion")] = '\0';
    
    printf("Message Déchiffré : %s\n", messageDecode);


    envoyer_recevoir("start", reponse);
    envoyer_recevoir("42", reponse);
    envoyer_recevoir(messageDecode, reponse);

    lire_clavier(message);  
    envoyer_recevoir(message, reponse); 

    printf("Réponse du Serveur : %s", reponse);

    printf("Fin de la Connexion au Serveur\n");
    return 0;
}
