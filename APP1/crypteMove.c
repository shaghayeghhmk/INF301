#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAXMSG MAXREP
void crypteMove(char *txt, char *enc) {
    while (strlen(txt) > 0) {
        char c = txt[0];
        int x = c % 8;
        txt++;
        strncat(enc, &c, 1); // Add 'c' to the end of the enc string
        
        if (strlen(txt) >= (unsigned long)x && x != 0) {
            char *temp = (char *)malloc((x + 1) * sizeof(char)); // Allocate memory for temp
            strncpy(temp, txt, x); // Copy the first x characters into temp
            temp[x] = '\0'; // Null-terminate temp
            
            // Remove the first x characters from txt
            memmove(txt, txt + x, strlen(txt) - x + 1);

            // Append temp to the end of txt
            strcat(txt, temp);
            
            free(temp); // Free the memory allocated for temp
        }
    }
}


int main() {
    char reponse[MAXREP];
    mode_debug(true);
    connexion("im2ag-appolab.u-ga.fr");
    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI", reponse);
    envoyer_recevoir("load crypteMove", reponse);
    envoyer_recevoir("help", reponse);
    char txt[MAXMSG];
    char enc[MAXMSG] = "";
    strcpy(txt, reponse);
    crypteMove(txt, enc);
    envoyer_recevoir("start", reponse);  
    envoyer_recevoir(enc, reponse);
    printf ("Réponse du serveur: %s", reponse);
    printf ("Fin de la connection au serveur\n");
    return 0;
}
