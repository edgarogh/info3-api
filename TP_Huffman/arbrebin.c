#include "arbrebin.h"
#include <stdio.h>
#include <stdlib.h>

/* implementation des operateurs de l'Arbre binaire */

/***************************************
 * ArbreVide                           *
 * parametres : aucun                  *
 * resultat : un Arbre                 *
 * description : renvoie un arbre vide *
 * effet de bord :aucun                *
 ***************************************/
Arbre ArbreVide() { return NULL; }

/**********************************************************************
 * NouveauNoeud                                                       *
 * parametres : les donnees : un Arbre g, un caractere c, un Arbre d  *
 * resultat : un Arbre                                                *
 * description : renvoie un nouvel Arbre dont la racine est etiquetee *
 * par c, de fils gauche g et de fils droit d                         *
 * effet de bord : une nouvelle cellule est allouee                   *
 **********************************************************************/
Arbre NouveauNoeud(Arbre g, Element c, Arbre d) {
    Arbre a = (Arbre)malloc(sizeof(struct cellule));

    a->etiq = c;
    a->fg = g;
    a->fd = d;

    return a;
}

/********************************************
 * EstVide                                  *
 * parametres : un Arbre a                  *
 * resultat : un booleen                    *
 * description : renvoie vrai si a est vide *
 * effet de bord : aucun                    *
 ********************************************/
int EstVide(Arbre a) { return (a == NULL); }

/******************************************
 * Etiq                                   *
 * parametres : un Arbre a                *
 * precondition : a non vide              *
 * resultat : un caractere                *
 * description : renvoie l'etiquette de a *
 * effet de bord : aucun                  *
 ******************************************/
Element Etiq(Arbre a) { return a->etiq; }

/*********************************************
 * FilsGauche                                *
 * parametres : un Arbre a                   *
 * precondition : a non vide                 *
 * resultat : un arbre                       *
 * description : renvoie le fils gauche de a *
 * effet de bord : aucun                     *
 *********************************************/
Arbre FilsGauche(Arbre a) { return a->fg; }

/********************************************
 * FilsDroit                                *
 * parametres : un Arbre a                  *
 * precondition : a non vide                *
 * resultat : un arbre                      *
 * description : renvoie le fils droit de a *
 * effet de bord : aucun                    *
 ********************************************/
Arbre FilsDroit(Arbre a) { return a->fd; }

/********************************************
 * LibererArbre                             *
 * parametres : un Arbre a                  *
 * resultat : aucun                         *
 * description : libere l'arbre a           *
 * effet de bord : a est detruit            *
 ********************************************/
void LibererArbre(Arbre a) {
    if (!EstVide(a)) {
        LibererArbre(FilsGauche(a));
        LibererArbre(FilsDroit(a));
        free(a);
    }
}

/**********************************************************************
 * EcrireArbre                                                        *
 * parametres : un descripteur de fichier, un Arbre a                 *
 * precondition : le fichier est ouvert en ecriture                   *
 * resultat : aucun                                                   *
 * description : ecrit l'arbre a dans le fichier, sous la forme d'une *
 * sequence de triplets (entier, entier representant l'arbre gauche,  *
 *  entier representant l'arbre droit). L'arbre vide est represente   *
 * par 0. La sequence est terminee par 0.                             *
 * effet de bord : ecriture dans le fichier                           *
 **********************************************************************/
int cpt_noeud;

int EcrireArbreRec(FILE *fichier, Arbre a) {
    int fg, fd;
    int racine;

    if (EstVide(a)) {
        /* Ne rien ecrire, renvoyer -1 */
        return -1;
    } else {
        /* Ecrire le fils gauche, fg = caractere a la racine du fils gauche */
        fg = EcrireArbreRec(fichier, FilsGauche(a));
        /* Ecrire le fils droit, fd = caractere a la racine du fils droit */
        fd = EcrireArbreRec(fichier, FilsDroit(a));
        /* Ecrire la sequence (caractere du noeud courant, fg, fd) */
        if (fg == -1) {
            /* Ecriture d'une feuille : le code de l'arbre est le code
               ascii du caractere */
            racine = (int)Etiq(a);
        } else {
            racine = cpt_noeud;
            cpt_noeud += 1;
        };
        fwrite(&racine, sizeof(int), 1, fichier);
        fwrite(&fg, sizeof(int), 1, fichier);
        fwrite(&fd, sizeof(int), 1, fichier);
        return racine;
    }
}

const int EOA = -1;

void EcrireArbre(FILE *fichier, Arbre a) {
    cpt_noeud = 256;
    EcrireArbreRec(fichier, a);
    /* sequence terminee par EOA (-1) */
    fwrite(&EOA, sizeof(int), 1, fichier);
}

/*********************************************************************
 * LireArbre                                                         *
 * parametres : un descripteur de fichier                            *
 * precondition : le fichier est ouvert en lecture, et contient a la *
 * position courante un arbre ecrit par EcrireArbre                  *
 * resultat : un arbre                                               *
 * description : lit l'arbre dans le fichier, dans lequel il a ete   *
 * ecrit par EcrireArbre.                                            *
 * effet de bord : le fichier a ete lu, un arbre a ete cree          *
 *********************************************************************/
Arbre LireArbre(FILE *fichier) {
    /* Stockage des noeuds crees */
    Arbre _TabArbre[513];
    /* M??me tableau que pr??c??demment, mais index?? de -1 ?? 511 au lieu de 0 ?? 512 */
    Arbre *TabArbre = _TabArbre + 1;
    int entierlu, fg, fd, racine;
    Element etiq;

    /* Initialisation : TabArbre[-1] contient l'arbre vide */
    TabArbre[-1] = ArbreVide();

    racine = -1;
    fread(&entierlu, sizeof(int), 1, fichier);
    while (entierlu != EOA) {
#ifdef DEBUG_ARBRE
        printf("entier lu : %d\n", entierlu);
#endif
        fread(&fg, sizeof(int), 1, fichier);
        fread(&fd, sizeof(int), 1, fichier);
#ifdef DEBUG_ARBRE
        printf("fg : %d\n", fg);
        printf("fd : %d\n", fd);
#endif
        /* Assertion : TabArbre[fg] et TabArbre[fd] ont ete affectes,
           eventuellement a l'arbre vide si fg=-1 ou fd=-1 */
        if (fg == -1) {
            etiq = (Element)entierlu;
        } else {
            /* Noeud interne, on met une ??tiquette quelconque */
            etiq = 'a';
        };
        TabArbre[entierlu] = NouveauNoeud(TabArbre[fg], etiq, TabArbre[fd]);
        racine = entierlu;
        /* Noeud suivant */
        fread(&entierlu, sizeof(int), 1, fichier);
    }
    return TabArbre[racine];
}

/*********************************************************************
 * AfficherArbre                                                     *
 * parametres : un arbre a                                           *
 * resultat : aucun                                                  *
 * description : affiche l'arbre a sur la sortie standard            *
 * effet de bord : un arbre est affiche                              *
 *********************************************************************/
void AfficherArbreRec(Arbre a, int indent) {
    int i;

    if (!EstVide(a)) {
        for (i = 0; i < indent; i++) {
            printf(" ");
        }
        printf("%c\n", Etiq(a));
        AfficherArbreRec(FilsGauche(a), indent + 2);
        AfficherArbreRec(FilsDroit(a), indent + 2);
    }
}

void AfficherArbre(Arbre a) { AfficherArbreRec(a, 0); }
