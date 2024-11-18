#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>

#include "stdes.h"

// tests pour la fonction lire() sur un fichier très long

int main(){
    FICHIER * f=ouvrir("fichiersTexte/Jekyll_et_Hyde.txt", 'L');   // fichier long avec texte sans espaces ni sauts à la ligne
    f->buffer_size=8;


    printf("=============== Lecture par élément de la taille du buffer système ===============\n");
    char p8[16];
    memset(p8, '\0', sizeof(p8));  // Remplir p avec 16 '\0' (au cas où il y a des trucs déjà écrit dans les emplacements mémoire...)
    int ret8;

    for(int i = 0; i < 50; i++){
        ret8 = lire(p8, 8, 1, f);
        assert(ret8 == 1);
        assert(strlen(p8) == 8);    // les données de p sont écrasées à chaque fois
        printf("%s\n", p8);
    }
    printf("OK\n\n");


    printf("=============== Lecture par élément d'une taille plus petite et mutliple du buffer système ===============\n");
    char p4[16];
    memset(p4, '\0', sizeof(p4));  // Remplir p avec 16 '\0' (au cas où il y a des trucs déjà écrit dans les emplacements mémoire...)
    int ret4;

    for(int i = 0; i < 50; i++){
        ret4 = lire(p4, 4, 1, f);
        assert(ret4 == 1);
        assert(strlen(p4) == 4);    // les données de p sont écrasées à chaque fois
        printf("%s\n", p4);
    }
    printf("OK\n\n");



    printf("=============== Lecture par élément d'une taille plus petite et non mutliple du buffer système ===============\n");
    char p6[16];
    memset(p6, '\0', sizeof(p6));  // Remplir p avec 16 '\0' (au cas où il y a des trucs déjà écrit dans les emplacements mémoire...)
    int ret6;

    for(int i = 0; i < 50; i++){
        ret6 = lire(p6, 6, 1, f);
        assert(ret6 == 1);
        //printf("len %ld\n", strlen(p6));
        assert(strlen(p6) == 6);    // les données de p sont écrasées à chaque fois
        printf("%s\n", p6);
    }
    printf("OK\n\n");


    printf("=============== Lecture par élément d'une taille plus grande que le double du buffer système ===============\n");
    char p16[17] = {0}; // p16[16] ne fonctionne pas (pb au niv. de la longueur et du contenu de la chaîne)
                        // pour une raison que j'ignore... read() renvoie le bon nombre de caractères et les copie 
                        // bien dans le buffer utilisateur (cf lire()) mais le caractère de fin de chaîne '\0'
                        // semble faire des siennes..
    int ret16;

    for(int i = 0; i < 50; i++){
        ret16 = lire(p16, 16, 1, f);
        assert(ret16 == 1);
        // printf("%ld\n", strlen(p16));
        assert(strlen(p16) == 16);
        printf("%s\n\n", p16);
    }
    printf("OK\n\n");


    printf("=============== Lecture par élément d'une taille encore plus grande que le double du buffer système ===============\n");
    char p30[30] = {0}; // et là ça marche...
    int ret30;

    for(int i = 0; i < 50; i++){
        ret30 = lire(p30, 30, 1, f);
        assert(ret30 == 1);
        // printf("%ld\n", strlen(p16));
        assert(strlen(p30) == 30);
        printf("%s\n\n", p30);
    }
    printf("OK\n\n");


    printf("=============== Lecture de 2 éléments de 7 octets ===============\n");
    char p7[15] = {0}; // même problème ici
    int ret7;

    for(int i = 0; i < 50; i++){
        ret7 = lire(p7, 7, 2, f);
        assert(ret7 == 2);
        // printf("%ld\n", strlen(p7));
        assert(strlen(p7) == 14);
        printf("%s\n\n", p7);
    }
    printf("OK\n\n");

    fermer(f);

    return 0;
}
