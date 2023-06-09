#include "../include/cmd.h"
#include "../include/noeud.h"
#include "../include/others.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*-------------------------------------------*/
void ls(noeud* courant){
    liste_noeud* fils=courant->fils;
    while(fils!=NULL){
        if(fils->no->est_dossier){
          printf("%s\n",fils->no->nom);
        }
        else{
          printf("%s\n",fils->no->nom);
        }
        fils=fils->succ;
    }
}
/*-------------------------------------------*/
noeud* cd_chem(noeud* courant, const char* chem){
    noeud* res=courant;
    char* c=capture(chem);
    if(c[0]=='/') res=courant->racine;
       if(c[1]!='/'){
       char* chemin = capture(chem);
       if (res->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(res, token)) {
                res = getAppartient(res, token);
                token = strtok(NULL, "/");
            } else {
                res=courant;
                perror("No such file or directory ssssd");
                free(chemin);
                free(c);
                exit(EXIT_FAILURE);
                break;
            }
         } while (token != NULL);
         free(chemin);
       } 
       else {
         free(chemin);
         free(c);
         perror("No such file or directory fffff");
         exit(EXIT_FAILURE);
       }
      }
      else{
        free(c);
        perror("No such file or directory ggggggggggg");
        exit(EXIT_FAILURE);
      }
    free(c);
    return res;
}
/*-------------------------------------------*/
noeud* cd(noeud* courant){
    return courant->racine;
}
/*-------------------------------------------*/
noeud* cd_point(noeud* courant){
    if(courant->pere==NULL){ perror("No such file or directory Sss"); exit(EXIT_FAILURE);}
    if(courant->pere!=NULL){ return courant->pere;}
    return NULL;
}
/*-------------------------------------------*/
void pwd(noeud* courant){
    noeud* c = courant;
    char path[1000] = "";
    char chaine[1000];
    char chaine2[99];
    
    if(c == c->racine){
        puts("/");
        return;
    }
    
    if (strlen(c->nom) > 0) {
        strcat(path, "/");
        strcat(path, c->nom);
    }
    
    strcpy(chaine, path);
    
    while(c->pere != c->racine){
        c = c->pere;
        strcpy(chaine2, chaine);
        strcpy(chaine, "");
        
        if (strlen(c->nom) > 0) {
            strcat(chaine, "/");
            strcat(chaine, c->nom);
        }
        strcat(chaine, chaine2);
    }
    
    printf("%s\n", chaine);
}
/*---------------------------------------------------------------*/
void mkdir(noeud* parent, const char *nom) {
    if(!estValide(nom)){
        perror("erreor nom invalide");
        exit(EXIT_FAILURE);
    }
    if(estValide(nom) && !existeDeja(parent,nom)){
    char *n=capture(nom);
    noeud *newDir=creerNoeud(true,n,parent,NULL,parent->racine);
    newDir->pere=parent;
    if (parent->fils == NULL) { 
        parent->fils = malloc(sizeof(liste_noeud));
        parent->fils->no = newDir;
        parent->fils->succ = NULL;
    } else {
        liste_noeud* ptr = parent->fils;
        while (ptr->succ != NULL) {
            ptr = ptr->succ;
        }
        ptr->succ = malloc(sizeof(liste_noeud));
        ptr->succ->no = newDir;
        ptr->succ->succ = NULL;
    }
    free(n);
    }
    else{
        perror("Ce dossier existe déjà!");
        exit(EXIT_FAILURE);
    }
}
/*-------------------------------------------*/
void touch(noeud* courant, const char* nom) {
    if(!estValide(nom)){
        perror("erreor nom invalide");
        exit(EXIT_FAILURE);
    }
    if (estValide(nom) && !existeDeja(courant, nom)) {
        char* c = capture(nom);
        noeud* newFic = creerNoeud(false, c, courant, NULL, courant->racine);
        if (courant->fils == NULL) {
            courant->fils = malloc(sizeof(liste_noeud));
            courant->fils->no = newFic;
            courant->fils->succ = NULL;
        }
        else {
            liste_noeud* liste = courant->fils;
            while (liste->succ != NULL) {
                liste = liste->succ;
            }
            liste->succ = malloc(sizeof(liste_noeud));
            liste->succ->no = newFic;
            liste->succ->succ = NULL;
        }
        free(c);
    }
    else {
        perror("Le nom est invalide ou un fichier portant ce nom existe déjà dans ce dossier.");
        exit(EXIT_FAILURE);
    }
}
/*-------------------------------------------*/
void rm(noeud* courant,const char* chem){
    noeud* toDelete = courant;   
    char* c=capture(chem);
    if(c[0]=='/') toDelete = courant->racine;
       if(c[1]!='/'){
    char* chemin = capture(chem);
    if (toDelete->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(toDelete, token)) {
                toDelete = getAppartient(toDelete, token);
                token = strtok(NULL, "/");
            } else {
                toDelete=courant;
                perror("No such file or directory GR SX");
                free(chemin);
                free(c);
                exit(EXIT_FAILURE);
            }
        } while (token != NULL);
        free(chemin);
    } else {
        free(chemin);
        free(c);
        perror("No such file or directory   ADZ E");
        exit(EXIT_FAILURE);
    }
    if(estParent(courant,toDelete)){
        perror("Vous ne pouvez pas supprimer un parent!");
        free(c);
        return;
    }
    liste_noeud* fils_de_pere=toDelete->pere->fils;
    free_chem(toDelete);
    liste_noeud* prev_fils = NULL;
    while(fils_de_pere != NULL){
        if(fils_de_pere->no == toDelete){
            if(prev_fils == NULL){
                toDelete->pere->fils = fils_de_pere->succ;
            } else {
                prev_fils->succ = fils_de_pere->succ;
            }
            free(fils_de_pere);
            break;
        }
        prev_fils = fils_de_pere;
        fils_de_pere = fils_de_pere->succ;
    }
    }
    else{
        free(c);
        perror("No such file or directory ggggggg");
        exit(EXIT_FAILURE);
      }
    free(c);
}
/*-------------------------------------------*/
void cp(noeud* courant,char* chemin_src, char* chemin_dest) {
    if (chemin_src == NULL || chemin_dest == NULL) {
        return;
        exit(EXIT_FAILURE);
    }
    noeud* src = rechercher_noeud(courant, chemin_src); //se place au dernier noeud du chemin source
    if (src == NULL) {
        printf("Erreur : Le chemin source n'existe pas.\n");
        exit(EXIT_FAILURE);
    }
    noeud* copie_src = copier_noeud(src); //copie tout les fils du dernier noeud source
    char* py=chemin_sans_dernier(chemin_dest); //prend tout le chemin sans le dernier dossier/fichier
    noeud* dest = rechercher_noeud(courant, py); //se place à l'avant dernier noeud du chemin destination
    if (dest == NULL) {
        printf("Erreur : Le chemin destination n'existe pas.\n");
        free_noeud(copie_src);
        exit(EXIT_FAILURE);
    }
    char* ky=chemin_dernier(chemin_dest); //ky prend le dernier nom du dossier/fichier du chemin de destination
    if (appartient_sous_arbre(src, dest)) {
    printf("Erreur : Le noeud destination est dans le sous-arbre du noeud source.\n");
    free_noeud(copie_src);
    exit(EXIT_FAILURE);
    }
    if(!appartient(dest,ky)){             //si par rapport au noeud courant, il n'y a pas de fils portant le nom de ky, on le crée.
    mkdir(dest,ky);
    dest=cd_chem(dest,ky);
    ajouter_fils(dest, copie_src->fils);
    free(py);
    }else{                               //sinon on avance dans ce dossier/fichier et on ajoute les fils 
    dest=cd_chem(dest,ky);
    ajouter_fils(dest, copie_src->fils);
    free(py);
    }
}
/*--------------------------------------------------------*/
void mv(noeud* courant,char* chem1, char* chem2){
    cp(courant,chem1,chem2);
    rm(courant,chem1);
}