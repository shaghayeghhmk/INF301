#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"

#define TAILLE_PILE 100 // Définir la taille maximale de la pile

// Structure représentant une pile
typedef struct {
    char* tab[TAILLE_PILE];
    int nb_elem;
} pile;

// Fonction pour empiler une chaîne de caractères sur la pile
// x est une adresse mémoire qui pointe vers un emplacement où est stocké un caractère ou une chaîne de caractères.
void empiler(pile *p, char *x) {
    if (p->nb_elem < TAILLE_PILE) {
        p->tab[p->nb_elem++] = x;
    } else {
        fprintf(stderr, "Débordement de pile !\n");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour dépiler une chaîne de caractères de la pile
char *depiler(pile *p) {
    if (p->nb_elem > 0) {
        return p->tab[--p->nb_elem];
    } else {
        fprintf(stderr, "Dépassement de pile !\n");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour afficher le contenu de la pile
void afficher_pile(const pile *p) {
    if (p->nb_elem == 0) {
        printf("La pile est vide !\n");
    } else {
        printf("Contenu de la pile: ");
        for (int i = p->nb_elem - 1; i >= 0; i--) {
            printf("%s ", p->tab[i]);
        }
    }
}

// Fonction pour arrêter le programme jusqu'à ce qu'une touche soit enfoncée
void stop(void) {
    char entrée = '\0';
    printf("Appuyez sur Entrée pour continuer...\n");
    while (entrée != '\r' && entrée != '\n') {
        entrée = getchar();
    }
}

// Fonction pour convertir un entier en chaîne de caractères
char* convertirEntierEnChaine(int nombre) {
    char* chaine = malloc(12 * sizeof(char));  // 12 est une taille suffisante pour stocker un entier
    sprintf(chaine, "%d", nombre);
    return chaine;
}

// Fonction principale pour interpréter une séquence d'instructions
int interprete(sequence_t* seq, bool debug) {
    pile p = {.nb_elem = 0}; // Initialiser la pile
    char commande;
    int x, y, ret, resultat, dir, arg1, nombreAcc, n;
    size_t index;
    char *F, *V;
    char *Commande;
    if (debug)
    {
        printf("Programme :");
        afficher(seq);
        printf("\n");
        stop();
    }

    cellule_t* courant = seq->tete;
    int final_resultat = 0;

    while (courant) {
        commande = courant->command;
        ret = 0;

        switch (commande) {
            case ' ':
                break;
            case 'A':
                ret = avance();
                if (ret == VICTOIRE) return VICTOIRE;
                if (ret == RATE) return RATE;
                break;
            case 'G':
                gauche();
                break;
            case 'D':
                droite();
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                empiler(&p, convertirEntierEnChaine(commande - '0'));
                break;
            case '+':
            case '-':
            case '*':
                if (p.nb_elem < 2) {
                    fprintf(stderr, "Pas assez d'opérandes sur la pile pour l'opération !\n");
                    exit(EXIT_FAILURE);
                }
                x = atoi(depiler(&p));
                y = atoi(depiler(&p));

                switch (commande) {
                    case '+':
                        resultat = y + x;
                        printf("Addition : %d + %d = %d\n", x, y, resultat);
                        break;
                    case '-':
                        resultat = y - x;
                        printf("Soustraction : %d - %d = %d\n", x, y, resultat);
                        break;
                    case '*':
                        resultat = y * x;
                        printf("Multiplication : %d * %d = %d\n", x, y, resultat);
                        break;
                    default:
                        fprintf(stderr, "Opérateur non pris en charge : '%c'\n", commande);
                        exit(EXIT_FAILURE);
                }

                empiler(&p, convertirEntierEnChaine(resultat));
                afficher_pile(&p);
                break;
            case 'M':
                dir = atoi(depiler(&p));
                // Effectuer une mesure
                resultat = mesure(dir);
                empiler(&p, convertirEntierEnChaine(resultat));
                printf("Mesure:%d\n", resultat);
                afficher_pile(&p);
                break;
            case 'P':
                arg1 = atoi(depiler(&p));
                if (arg1 == 0) {
                    pose(0);
                } else {
                    pose(arg1);
                }
                afficher_pile(&p);
                break;
            case '{':
                nombreAcc = 1;
                courant = courant->suivant;
                index = 0;
                char *tmp = malloc(TAILLE_PILE * sizeof(char));
                while (courant->suivant != NULL) {
                    if (courant->command == '{') {
                        nombreAcc++;
                    }
                    if (courant->command == '}') {
                        nombreAcc--;
                        if (nombreAcc == 0) {
                            break;
                        }
                    }
                    tmp[index] = courant->command;
                    index++;
                    courant = courant->suivant;
                }
                empiler(&p, tmp);
                afficher_pile(&p);
                break;
            case '?':
                F = depiler(&p);
                V = depiler(&p);
                n = atoi(depiler(&p));
                    if (n == 0) {
                        Commande = F;
                    } else {
                        Commande = V;
                    }
                sequence_t *nouvelleSeq = malloc(sizeof(sequence_t));
                conversion(Commande, nouvelleSeq);
                ret = interprete(nouvelleSeq, debug);
                printf("%d \n", ret);
                afficher_pile(&p);
                if (ret == VICTOIRE) {
                    return VICTOIRE;
                }
                if (ret == RATE) {
                    return RATE; 
                }                
                break;
            case 'Z':
                if (p.nb_elem > 0) {
                    char *chaine = depiler(&p);
                    int longueur = strlen(chaine);

                    // Inverser la chaîne
                    for (int i = 0; i < longueur / 2; i++) {
                        char temp = chaine[i];
                        chaine[i] = chaine[longueur - i - 1];
                        chaine[longueur - i - 1] = temp;
                    }

                    // Empiler la chaîne inversée
                    empiler(&p, chaine);
                } else {
                    fprintf(stderr, "Pas assez d'éléments sur la pile pour la commande 'Z' !\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'X':
                if (p.nb_elem >= 2) {
                    char *a = depiler(&p);
                    char *b = depiler(&p);
                    empiler(&p, a);
                    empiler(&p, b);
                } else {
                    fprintf(stderr, "Pas assez d'éléments sur la pile pour l'échange !\n");
                    printf("Taille de la pile : %d\n", p.nb_elem);
                    exit(EXIT_FAILURE);
                }
                break;
            case '!':
                if (p.nb_elem > 0) {
                    char *a = depiler(&p);
                    sequence_t *nouvelleSeq = malloc(sizeof(sequence_t));
                    conversion(a, nouvelleSeq);
                    int ret = interprete(nouvelleSeq, debug);
                    if (ret == VICTOIRE){
                        return VICTOIRE;
                    }
                    if (ret == RATE)   {
                        return RATE;
                }
            }
                else {
                    fprintf(stderr, "La pile est vide, impossible d'exécuter une commande !\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'B':
                if (p.nb_elem >= 2){
                int n = atoi(depiler(&p));
                char *cmd = depiler(&p);
                while (n > 0) {
                    sequence_t *nouvelleSeq = malloc(sizeof(sequence_t));
                    conversion(cmd, nouvelleSeq);
                    int ret = interprete(nouvelleSeq, debug);
                    if (ret == VICTOIRE) {
                        return VICTOIRE; /* on a atteint la cible */
                    }
                    if (ret == RATE) {
                        return RATE; /* on a atteint la cible */
                    }
                    n--;
                    empiler(&p, cmd);
                    empiler(&p, convertirEntierEnChaine(n));
                }
                break;
                } else {
                    fprintf(stderr, "Pas assez d'éléments sur la pile pour la boucle !\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'C':
                if (p.nb_elem > 0) {
                    char *e = depiler(&p);
                    empiler(&p, e);
                    empiler(&p, e);  // Clone
                } else {
                    fprintf(stderr, "La pile est vide, impossible de cloner un élément !\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'I':
                if (p.nb_elem > 0) {
                    depiler(&p);  // Ignore l'élément en haut de la pile
                } else {
                    fprintf(stderr, "La pile est vide, impossible d'ignorer un élément !\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'R':
                if (p.nb_elem >= 3) {
                    int step = atoi(depiler(&p)); 
                    int count = atoi(depiler(&p)); 
                    int stackSize = p.nb_elem;
                    int iteration = 0;
                    while (iteration < step) {
                        char *temp = p.tab[stackSize - count];
                        for (int i = stackSize - count; i < stackSize; i++) {
                            p.tab[i] = p.tab[i + 1];
                        }
                        p.tab[stackSize - 1] = temp;
                        iteration++;
                    }
                }
                break;
            default:
                eprintf("Caractère inconnu : '%c'\n", commande);
                break;
        }

        // Affichage pour le débogage
        afficherCarte();
        printf("Programme :");
        afficher(seq);
        printf("\n");

        if (debug)
            stop();

        courant = courant->suivant;
    }
    // À la fin de la routine, le résultat souhaité est le dernier élément de la pile
    printf("calcul : %d\n", final_resultat);
    return CIBLERATEE;
}