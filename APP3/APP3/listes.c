#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t* L) {
    if (L != NULL) {
        L->tete = NULL;
    }
}

void liberer_liste(liste_t* L) {
    if (L == NULL) {
        return;  // Vérification des paramètres
    }

    // Pointeur temporaire pour parcourir la liste
    cellule_t* courant = L->tete;
    cellule_t* suivant;

    // Parcours de la liste et libération de chaque maillon
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant->val);  // Libération de la mémoire de la valeur
        free(courant);          // Libération du maillon
        courant = suivant;
    }

    // La liste est maintenant vide
    L->tete = NULL;
}


int ajouter_en_queue(liste_t* L, string c) {
    if (L == NULL || c == NULL) {
        return 1;  // Vérification des paramètres
    }

    // Créer un nouveau maillon
    cellule_t* nouveau_maillon = malloc(sizeof(cellule_t));
    if (nouveau_maillon == NULL) {
        return 1;  // Échec de l'allocation mémoire
    }

    // Copier la valeur dans le nouveau maillon
    nouveau_maillon->val = strdup(c);
    nouveau_maillon->suivant = NULL;

    // Si la liste est vide, le nouveau maillon devient la tête
    if (L->tete == NULL) {
        L->tete = nouveau_maillon;
    } else {
        // Sinon, trouver le dernier maillon et ajouter le nouveau maillon à la fin
        cellule_t* dernier = L->tete;
        while (dernier->suivant != NULL) {
            dernier = dernier->suivant;
        }
        dernier->suivant = nouveau_maillon;
    }
    return 0;  
}


/*

int ajouter_en_tete(liste_t* L, arbre c) {
    // Vérification des paramètres
    if (L == NULL || c == NULL) {
        return;
    }

    // Créer un nouveau maillon
    cellule_t* nouveau_maillon = malloc(sizeof(cellule_t));
    if (nouveau_maillon == NULL) {
        return;
    }

    // Initialiser la valeur du nouveau maillon
    nouveau_maillon->val = c->valeur;

    // Initialiser le lien suivant à NULL
    nouveau_maillon->suivant = NULL;

    // Si la liste est vide, ajouter le maillon en tête
    if (liste_est_vide(L)) {
        L->tete = nouveau_maillon;
    } else {
        // Sinon, parcourir la liste jusqu'à la fin et ajouter le maillon
        cellule_t* courant = L->tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau_maillon;
    }
}
*/

int ajouter_tete(liste_t* L, string c) { /* retourne 0 si OK, 1 sinon  */
    if (L == NULL || c == NULL) {
        return 1;  // Vérification des paramètres
    }

    // Créer un nouveau maillon
    cellule_t* nouveau_maillon = malloc(sizeof(cellule_t));
    if (nouveau_maillon == NULL) {
        return 1;  // Échec de l'allocation mémoire
    }

    // Copier la valeur dans le nouveau maillon
    nouveau_maillon->val = strdup(c);

    // Mettre à jour les liens
    nouveau_maillon->suivant = L->tete;
    L->tete = nouveau_maillon;

    return 0;  // Succès
}
/*
int liste_est_vide(liste_t* L) {
    return (L == NULL || L->tete == NULL);
}

int taille_liste(liste_t* L) {
    if (L == NULL) {
        return 0;
    }

    int taille = 0;
    cellule_t* courant = L->tete;

    while (courant != NULL) {
        taille++;
        courant = courant->suivant;
    }

    return taille;
}


arbre supprimer_tete(liste_t* L) {
    if (liste_est_vide(L)) {
        return NULL; // La liste est vide, rien à supprimer
    }

    cellule_t* ancienne_tete = L->tete;
    arbre valeur = ancienne_tete->val;

    // Mettre à jour la tête de la liste
    L->tete = ancienne_tete->suivant;

    // Libérer la mémoire de l'ancienne tête
    free(ancienne_tete);

    return valeur;
}*/

