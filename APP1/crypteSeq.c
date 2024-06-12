#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAXMSG MAXREP

void supprimerDeSequence(char *sequence, char c) {
    int longueur = strlen(sequence);
    int i, j;

    for (i = 0; i < longueur; i++) {
        if (sequence[i] == c) {
            for (j = i; j < longueur - 1; j++) {
                sequence[j] = sequence[j + 1];
            }
            longueur--;
            i--;
        }
    }
    sequence[longueur] = '\0';
}

void crypteSeq(char *message) {
    char sequence[MAXMSG] = ""; // Tableau pour stocker la séquence de caractères
    char crypte[MAXMSG] = "";   // Tableau pour stocker le message crypté
    bool caractereDejaVu[MAXMSG] = {false}; // Tableau pour suivre les caractères déjà rencontrés
    char sequencePrecedente[MAXMSG] = ""; // Copie de la séquence de l'étape précédente

    int i, j;

    for (i = 0; message[i] != '\0'; i++) {
        char c = message[i];

        // Copie de la séquence précédente pour utilisation ultérieure
        strcpy(sequencePrecedente, sequence);

        // Si le caractère c a déjà été rencontré, le traitement est différent
        if (caractereDejaVu[c] == true) {
            // Recherche le caractère dans la séquence et le supprime
            supprimerDeSequence(sequence, c);

            // D qui est soit :
            // - le caractere qui precede C dans la sequence
            // - le dernier caractere de la sequence si C est le premier caractere de la sequence
            char d;
            // Recherche le caractère 'd' dans la séquence précédente
            for (j = 0; sequencePrecedente[j] != '\0'; j++) {
                if (sequencePrecedente[j] == c) {
                    d = j > 0 ? sequencePrecedente[j - 1] : sequencePrecedente[strlen(sequencePrecedente) - 1];
                    break;
                }
            }

            // On remplace c par D dans le message crypté
            crypte[strlen(crypte)] = d;

            sequence[strlen(sequence)] = c;
        } else {
            // Ajoute c à la séquence
            sequence[strlen(sequence)] = c;
            // Ajoute le caractère précédent au message crypté
            crypte[strlen(crypte)] = c;
            // Marque le caractère comme déjà vu
            caractereDejaVu[c] = true;
        }

        // Affiche le caractère c et le caractère d à chaque étape
        //printf("Étape %d : c = %c, d = %c (Séquence : %s, Crypté : %s)\n", i + 1, c, crypte[strlen(crypte) - 1], sequence, crypte);
    }

    // Copie le message crypté dans le message d'origine
    strcpy(message, crypte);
}

void decrypteSeq(char *message) {
    char sequence[MAXMSG] = ""; // Tableau pour stocker la séquence de caractères
    char crypte[MAXMSG] = "";   // Tableau pour stocker le message crypté
    bool caractereDejaVu[MAXMSG] = {false}; // Tableau pour suivre les caractères déjà rencontrés
    char sequencePrecedente[MAXMSG] = ""; // Copie de la séquence de l'étape précédente

    int i, j;

    for (i = 0; message[i] != '\0'; i++) {
        char c = message[i];

        // Copie de la séquence précédente pour utilisation ultérieure
        strcpy(sequencePrecedente, sequence);

        // Si le caractère c a déjà été rencontré, le traitement est différent
        if (caractereDejaVu[c] == true) {
            // Recherche le caractère dans la séquence et le supprime
            supprimerDeSequence(sequence, c);

            // D qui est soit :
            // - le caractere qui precede C dans la sequence
            // - le dernier caractere de la sequence si C est le premier caractere de la sequence
            char d;
            // Recherche le caractère 'd' dans la séquence précédente
            for (j = 0; sequencePrecedente[j] != '\0'; j++) {
                if (sequencePrecedente[j] == c) {
                    if (j == (strlen(sequencePrecedente) - 1)) {
                        // c est le dernier caractère dans la séquence
                        d = sequencePrecedente[j - 1];
                    } else {
                        // c n'est pas le dernier caractère dans la séquence
                        d = sequencePrecedente[j + 1];
                    }
                    break;
                }
            }    
            // On remplace c par D dans le message crypté
            crypte[strlen(crypte)] = d;

            sequence[strlen(sequence)] = c;
        } else {
            // Ajoute c à la séquence
            sequence[strlen(sequence)] = c;
            // Ajoute le caractère précédent au message crypté
            crypte[strlen(crypte)] = c;
            // Marque le caractère comme déjà vu
            caractereDejaVu[c] = true;
        }

        // Affiche le caractère c et le caractère d à chaque étape
        //printf("Étape %d : c = %c, d = %c (Séquence : %s, Crypté : %s)\n", i + 1, c, crypte[strlen(crypte) - 1], sequence, crypte);
    }

    // Copie le message crypté dans le message d'origine
    strcpy(message, crypte);
}



void decrypteMove(char *enc, char *txt) {
    while (strlen(enc) > 0) {
        char c = enc[strlen(enc) - 1]; // Prendre le dernier caractère de ENC
        int x = c % 8;
        
        if (strlen(txt)> x ) {
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
    char message[MAXMSG]; 

    mode_debug(true);

    connexion("im2ag-appolab.u-ga.fr");

    envoyer_recevoir("login 12200069 HAJMOHAMMADKASHI", reponse);
    envoyer_recevoir("load crypteSeq", reponse);
    char decodedText[MAXMSG]="";
    envoyer_recevoir("start", reponse);
    decrypteMove(reponse,decodedText);
    printf("%s",decodedText);

    crypteSeq(decodedText);
    envoyer_recevoir(decodedText, reponse);

    //printf("Message final crypté : %s\n", reponse);
    //char message2[MAXMSG] = "Alice,\nSl tuiaraSvcsuusAruvaArrmcvevgmAcr'gmS,qS'tsqem'uiqtaulati,on rS staAS,s\nmvivqhScvdmmmmnoypi'ldmdldoguycumtoy.gCpaodleinod.nCaaas'ro Sojl,iousC eubajlsSSinguovuicavnglbonbbSqlstfCshscssndn.Jt.gJucpJc'sspfqpeunroq umqsmmbqfllsnvofrorrimeilr o-nc.citAumtmiqv-rms-afturnt'at\nub f eegiiasod-itmjntuu-lnvmvv?c.e.ddnmbAuibtad.evddtjEulsv-vgSmfubvtbeIru?-isl-lveJhettttaltpeIeeql Csluht-rthbeteiedlCld,Ieeegeai,I-crIc\nIethpcpp\n'oosIdm\n'eplqIRd'j'u'Afpbrnhppi'cRx'c,orxslNmxb,wNwmn ue.AaiNSRNi.attmAlnlAnnnnvetJc,xSEquiabo\n-edbpjbmn,yCwe,et,Egg,E,eggrd,,eiltggwaddajv\np\noe:twNvenh:rttiuEsmgvllhrpgdoi:wbBtq''";
    decrypteSeq(decodedText);
    printf("%s",decodedText);


    printf ("Fin de la connection au serveur\n");
    return 0;
}