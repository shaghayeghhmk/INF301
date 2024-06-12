#include "client.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // Pour malloc et free


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

void decrypteMove(char *enc, char *txt) {
    while (strlen(enc) > 0) {
        char c = enc[strlen(enc) - 1]; // Prendre le dernier caractère de ENC
        int x = c % 8;
        
        if ((unsigned long)strlen(txt) > (unsigned long)x ) {
            // Extraire les X derniers caractères de ENC et les déplacer au début de TXT
            char temp[MAXMSG]; // Assurez-vous que la taille est suffisamment grande
            strncpy(temp, txt + strlen(txt) - x, x); // Copier les X derniers caractères dans temp
            temp[x] = '\0'; // Null-terminate temp
            // Supprimer les X derniers caractères de TXT
            txt[strlen(txt) - x] = '\0';
            
            // Ajouter C au début de TXT
            strcat(temp, txt);
            strcpy(txt, temp);
            for(int i=strlen(txt); i>0;i--){
                txt[i]=txt[i-1];
            }
            txt[0]=c;
        }
        else { 
            for(int i=strlen(txt); i>0;i--){
                txt[i]=txt[i-1];
            }
            txt[0]=c;

            }

        
        // Supprimer le dernier caractère de ENC
        enc[strlen(enc) - 1] = '\0';
    }

}

int main() {
    char reponse[MAXREP];

    mode_debug(true);

    connexion("im2ag-appolab.u-ga.fr");
    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI", reponse);
    envoyer_recevoir("load BayOfPigs", reponse);
    envoyer_recevoir("help", reponse);
    envoyer_recevoir("start", reponse);

    // Assuming the received text after "start" is stored in reponse, decrypt it
    char decodedText1[MAXMSG] = "";
    char decodedText2[MAXMSG] = "";
    decrypteMove(reponse, decodedText1);
    // Send the decoded text back to the server;
    char codedText[MAXREP]="Patria o muerte";
    char decodedText[MAXREP]="";
    crypteMove(codedText, decodedText);
    printf("%s",decodedText1);
    envoyer_recevoir( decodedText,reponse);

    decrypteMove(reponse,decodedText2);
    printf("%s",decodedText2);


    printf("Fin de la connection au serveur\n");
    return 0;
}
