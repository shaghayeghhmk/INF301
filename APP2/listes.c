#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


bool silent_mode = false;

cellule_t* nouvelleCellule(void) {
    cellule_t* newcel = malloc(sizeof(struct cellule));
    if (newcel == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la cellule.\n");
        exit(EXIT_FAILURE);
    }
    newcel->command = 0;  // Initialiser à une valeur par défaut
    newcel->suivant = NULL;
    return newcel;
}

void detruire_cellule(cellule_t* cel) {
    free(cel);
}

cellule_t* creer_cellule(char commande) {
    cellule_t* c = nouvelleCellule();
    c->command = commande;
    return c;
}

void ajout_queue(sequence_t *l, char comm)
{
    cellule_t *c, *v;
    c = malloc(sizeof(struct cellule));
    c->command = comm;
    c->suivant = NULL;
    v = l->tete;
    if (v != NULL)
    {
        while (v->suivant != NULL)
        {
            v = v->suivant;
        }
        v->suivant = c;
    }
    else
    {
        l->tete = c;
        c->suivant = NULL;
    }
}

void conversion(char *texte, sequence_t *seq)
{
    seq->tete = NULL;
    for (int i = 0; i < (int)strlen(texte); i++)
    {
        char cr = texte[i];
        ajout_queue(seq, cr);
    }
}

void afficher(sequence_t *seq)
{
    assert(seq); /* Le pointeur doit être valide */

    cellule_t *current = seq->tete;
    while (current != NULL)
    {
        printf("%c", current->command);
        current = current->suivant;
    }
}

// sequence_t* creer_sequence(const char commande) {
//     sequence_t* seq = malloc(sizeof(sequence_t));
//     if (seq == NULL) {
//         fprintf(stderr, "Erreur d'allocation de mémoire pour la séquence.\n");
//         exit(EXIT_FAILURE);
//     }
//     seq->tete = creer_cellule(commande);
//     return seq;
// }

// void detruire_sequence(sequence_t* seq) {
//     if (seq != NULL) {
//         // Libérer la mémoire pour les cellules suivantes
//         cellule_t* current = seq->tete;
//         while (current != NULL) {
//             cellule_t* next = current->suivant;
//             detruire_cellule(current);
//             current = next;
//         }

//         // Libérer la mémoire pour la séquence
//         free(seq);
//     }
// }

// // Ajoutez cette fonction à votre fichier listes.c
// void afficher_sequence(sequence_t* seq) {
//     assert(seq);

//     cellule_t* current = seq->tete;
//     printf("Sequence : ");
//     while (current != NULL) {
//         printf("%c", current->command);
//         current = current->suivant;
//     }
//     printf("\n");
// }
