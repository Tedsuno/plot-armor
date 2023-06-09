#include "../include/cmd.h"
#include "../include/noeud.h"
#include "../include/others.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Fonction qui permet de savoir si "ch" est une commande valide ou non
bool is_commande(char* ch){
    if(ch!=NULL){
    if(strcmp(ch,"ls")!=0 && strcmp(ch,"pwd")!=0 && strcmp(ch,"mkdir")!=0 && strcmp(ch,"cp")!=0 && strcmp(ch,"rm")!=0
        && strcmp(ch,"cd")!=0 && strcmp(ch,"mv")!=0 && strcmp(ch,"touch")!=0 && strcmp(ch,"print")!=0 )
        return false;
    }
    return true; 
}

// Fonction qui permet de découper une ligne mot par mot "count" fois à chaque fois qu'elle rencontre un espace ou un saut de ligne
char* commande_terminale(char* phrase, int count) {
    char* resultat = malloc(strlen(phrase) + 1);
    strcpy(resultat, phrase);
    char* token;
    // Variable temporaire pour stocker la valeur de resultat (car on utilise strtok -> free -> fuite mémoire)
    char* temp_resultat = resultat;
    // On découpe mot par mot ici grâce à strtok()
    for (int i = 0; i < count; i++) {
        if (resultat != NULL) {
            token = strtok(resultat, " \n\r");
            resultat = strtok(NULL, "");
        } else {
            free(temp_resultat);
            return NULL;  // Renvoie NULL si le nombre de mots est dépassé
        }
    }
    if (token != NULL) {
        char* copie_token = malloc(strlen(token) + 1);
        strcpy(copie_token, token);
        free(temp_resultat);
        return copie_token;
    } else {
        free(temp_resultat);
        return NULL;
    }
}


/*--------------------------------------------------------*/
int main(int argc, char *argv[]){

    noeud* racine=creerRacine();
    noeud* courant=racine;

    if (argc != 2) {
        printf("Usage: %s fichier\n", argv[0]);
        free(racine);
        return 1;
    }

    char ch[51];
    FILE* flux = fopen(argv[1], "r");

    if (flux == NULL) {
    perror("Probleme ouverture de fichier");
    }

    while (fgets(ch, 50, flux) != NULL) {

        char* com1=commande_terminale(ch,1); // Premier mot de la ligne (représentant le nom de la commande)
        char* com2=commande_terminale(ch,2); // Deuxieme mot de la ligne (représentant le chemin1)
        char* com3=commande_terminale(ch,3); // Premier mot de la ligne (représentant le chemin2)
        char* com4=commande_terminale(ch,4); // Premier mot de la ligne (représentant le chemin3 qui n'existe (normalement) pas)
        // On vérifie si "com1" est une commande valide
        if(!is_commande(com1)){
            printf("Vous avez entré une commande inconnue : %s",com1);
            free_chem(racine);
            exit(EXIT_FAILURE);
        }

        if(com1!=NULL){

        // Puis on effectue les commandes selon les valeurs de "com1","com2","com3"
        // A noter que s'il existe un chemin supplémentaire là ou il ne devrait pas y en avoir (exemple : cd xx YY), erreur !
        if(strcmp(com1,"touch")==0){
            if(com2!=NULL && com3==NULL){
            touch(courant,com2);
            }
            else{
                perror("erreur [touch] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE);  
            }
        }

        if(strcmp(com1,"mkdir")==0){
            if(com2!=NULL && com3==NULL){
                mkdir(courant,com2);
            }
            else{
                perror("erreur [mkdir] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE);  
            }
        }
         if(strcmp(com1,"print")==0){
            if(com2==NULL){
                printf("-----print-----\n");
                noeud* bis=courant;
                courant=courant->racine;
                print_arbre(courant);
                courant=bis;
                printf("---------------\n");
            }
            else{
                perror("erreur [print] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE);  
            }
        }
        if(strcmp(com1,"mv")==0){
            if(com2!=NULL && com3!=NULL && com4==NULL) mv(courant,com2,com3);
            else{   
                perror("erreur [mv] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE); 
            }
        }

        if(strcmp(com1,"cp")==0){
            if(com2!=NULL && com3!=NULL && com4==NULL) cp(courant,com2,com3);
            else{ 
                perror("erreur [cp] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE); 
            }
        } 

        if(strcmp(com1,"ls")==0){
            if(com2==NULL){
                printf("-----ls-----\n");
                ls(courant);
                printf("---------------\n");
            }
            else{
                perror("erreur [ls] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE);  
            }
        }
        if(strcmp(com1,"rm")==0){
            if(com2!=NULL && com3==NULL){
                rm(courant,com2);
            }
            else{
                perror("erreur [rm] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE);  
            }
       }

        if(strcmp(com1,"pwd")==0){
            if(com2==NULL){
                printf("------pwd------\n");
                pwd(courant);
                printf("---------------\n");
            }
            else{
                perror("erreur [pwd] ne respecte pas le format de base."); free_chem(racine);
                free(racine); exit(EXIT_FAILURE);  
            }
        }

        if(strcmp(com1, "cd") == 0){
            if(com2==NULL){
                courant=cd(courant);
            }
            else if(com3==NULL){
                if(strcmp(com2, "..") == 0) courant=cd_point(courant);
                if(strcmp(com2, "..") != 0) courant=cd_chem(courant,com2);
            } 
            else{
                perror("erreur [cd] ne respecte pas le format de base.");
                free_chem(racine);
                free(racine);
                exit(EXIT_FAILURE);
            }  
          }
        }
            free(com1);
            free(com2);
            free(com3);
            free(com4);
    }

    int r = fclose(flux);
    if (r != 0) {
        perror("Probleme fermeture de fichier");
    }
    free_chem(racine);
    free(racine);
    return 0;
}