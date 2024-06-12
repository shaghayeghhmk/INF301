#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"


#include <execinfo.h> //

/*ACTE I*/

void analyse_arbre_recursif(arbre racine, int* nb_esp, int* nb_carac) {
    // Cas de base : si la racine est vide (NULL)
    if (racine == NULL) {
        return;
    }

    // Cas : si la racine est une feuille (ni gauche ni droit)
    else if (racine->gauche == NULL && racine->droit == NULL) {
        *nb_esp += 1;
        return;
    }

    // Cas : si la racine a à la fois un sous-arbre droit et un sous-arbre gauche
    else if (racine->gauche != NULL && racine->droit != NULL) {
        *nb_carac += 1; // Ajouter 1 pour la caractéristique de la racine
        analyse_arbre_recursif(racine->gauche, nb_esp, nb_carac);
        analyse_arbre_recursif(racine->droit, nb_esp, nb_carac);
    }
    else if (racine->gauche == NULL && racine->droit != NULL){
        *nb_carac +=1 ;
        analyse_arbre_recursif(racine->droit, nb_esp, nb_carac);
    }
    else if (racine->gauche != NULL &&racine->droit == NULL){
        *nb_carac +=1 ;
        analyse_arbre_recursif(racine->gauche, nb_esp, nb_carac);
    }
}

void analyse_arbre(arbre racine, int* nb_esp, int* nb_carac) {
    // Initialiser les compteurs à 0
    *nb_esp = 0;
    *nb_carac = 0;

    // Appel à la fonction récursive
    analyse_arbre_recursif(racine, nb_esp, nb_carac);
}

/*ACTE I B

void generer_dot(arbre racine, FILE* fichier_dot) {
    fprintf(fichier_dot, "digraph arbre {\n");
    generer_dot_recursif(racine, fichier_dot);
    fprintf(fichier_dot, "}\n");
}

void generer_dot_recursif(arbre racine, FILE* fichier_dot) {
    if (racine == NULL) {
        return;
    }

    // Générer un nœud pour la valeur de la racine
    fprintf(fichier_dot, "  \"%p\" [label=\"%s\"];\n", (void*)racine, racine->valeur);

    // Générer les liens vers les sous-arbres gauche et droit
    if (racine->gauche != NULL) {
        fprintf(fichier_dot, "  \"%p\" -> \"%p\" [label=\"gauche\"];\n", (void*)racine, (void*)racine->gauche);
        generer_dot_recursif(racine->gauche, fichier_dot);
    }

    if (racine->droit != NULL) {
        fprintf(fichier_dot, "  \"%p\" -> \"%p\" [label=\"droit\"];\n", (void*)racine, (void*)racine->droit);
        generer_dot_recursif(racine->droit, fichier_dot);
    }
}
*/

/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */

int estFeuille(arbre a)
{
    return !(a->gauche) && !(a->droit);
}

int rechercher_espece(arbre racine, char* espece, liste_t* seq) {
    if (racine == NULL) {
        return 1; // L'espèce n'est pas référencée dans l'arbre
    }
    else if (estFeuille(racine)){
        if (strcmp(racine->valeur, espece) == 0) {
            liberer_liste(seq);
            return 0;
        } else { 
            return 1;
            }
    } else {
        // Recherche dans les sous-arbres gauche et droit
        if (rechercher_espece(racine->gauche, espece, seq) == 0) {
            return 0; // L'espèce a été retrouvée dans le sous-arbre gauche
        } else if (rechercher_espece(racine->droit, espece, seq) == 0) {
            ajouter_tete(seq, racine->valeur);
            return 0; // L'espèce a été retrouvée dans le sous-arbre droit
        } 
        return 1; // L'espèce n'a pas été retrouvée
    }
}



/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un 
 * message d'erreur.
 */
int recherche_carc(arbre racine, char *car){
	if (racine==NULL){
		return 0;
	}
	if (strcmp(racine->valeur,car)==0){
		return 1;
	}
	if (recherche_carc(racine->gauche,car)){
		return 1;
	}
	if (recherche_carc(racine->droit,car)){
		return 1;
	}
	return 0;
}


noeud* nouveau_noeud_valeur(char* valeur) {
    noeud* n = (noeud*)malloc(sizeof(noeud));
    if (n != NULL) {
        n->valeur = strdup(valeur); // Assurez-vous de libérer cette chaîne ultérieurement
        n->gauche = NULL;
        n->droit = NULL;
        n->nb_caracteristiques = 0;
    }
    return n;
}

int ajouter_espece(arbre* racine, char* espece, cellule_t* seq) {
    if (*racine == NULL) {
        // Si l'arbre est vide, créez un nouveau nœud avec la première valeur
        *racine = nouveau_noeud_valeur(seq->val);
        seq = seq->suivant;
    }

    arbre courant = *racine;

    while (seq->suivant != NULL) {
        if (recherche_carc(courant, seq->val)) {
            // La caractéristique est déjà présente, passez à la suivante
            seq = seq->suivant;
        } else {
            if (courant->gauche == NULL) {
                // Ajoutez une nouvelle caractéristique à gauche
                courant->gauche = nouveau_noeud_valeur(seq->val);
                seq = seq->suivant;
            } else {
                // La caractéristique n'est pas présente, ajoutez un nouveau nœud
                courant = courant->gauche;
            }
        }
    }

    // Ajoutez la dernière caractéristique à gauche
    if (!recherche_carc(courant, seq->val)) {
        courant->gauche = nouveau_noeud_valeur(seq->val);
    }

    // Ajoutez l'espèce à droite
    // Vérifiez si l'espèce existe déjà
    if (rechercher_espece(courant->droit, espece, NULL) == 0) {
        fprintf(stderr, "Ne peut ajouter %s : possède les mêmes caractères que %s.\n", espece, courant->droit->valeur);
        return 1; // Retourne un code d'erreur
    } else {
        courant->droit = nouveau_noeud_valeur(espece);
        return 0; // Succès
    }
}











/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
 /*
int hauteur(arbre a){
if (a==NULL){
	return -1;
}
int hgauche, hdroite, max;
hgauche=hauteur(a->gauche);
hdroite=hauteur(a->droit);
if (hgauche>hdroite){
	 max=hgauche;
}else{
	max=hdroite;
}
return max+1;
}
void afficher_niv_rec(arbre racine,int n_actuel,int n, FILE *fout ){
    if (racine == NULL ){
		return;
	}
	if ((n == n_actuel)&&(!estFeuille(racine))){
		fprintf(fout,"%s ",racine->valeur);
		return;
	}
	afficher_niv_rec(racine->gauche,n_actuel+1,n,fout);
	afficher_niv_rec(racine->droit,n_actuel+1,n,fout);
}*/

// Acte 4


int ajouter_carac(arbre* a, char* carac, cellule_t* seq) {
   printf ("<<<<< À faire: fonction ajouter_carac fichier " __FILE__ "\n >>>>>");
   return 0;
}
