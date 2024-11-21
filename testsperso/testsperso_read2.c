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
    char p1[16] = {0};  // au cas où il y a des trucs déjà écrit dans les emplacements mémoire...
    int ret1;

    for(int i = 0; i < 50; i++){
        ret1 = lire(p1, 8, 1, f);
        assert(ret1 == 1);
        printf("%s\n", p1);
    }
    printf("OK\n\n");


    printf("=============== Lecture par élément d'une taille plus petite et mutliple du buffer système ===============\n");
    char p2[16] = {0};
    int ret2;

    for(int i = 0; i < 50; i++){
        ret2 = lire(p2, 4, 1, f);
        assert(ret2 == 1);
        printf("%s\n", p2);
    }
    printf("OK\n\n");



    printf("=============== Lecture par élément d'une taille plus petite et non mutliple du buffer système ===============\n");
    char p3[16] = {0};
    int ret3;

    for(int i = 0; i < 50; i++){
        ret3 = lire(p3, 6, 1, f);
        assert(ret3 == 1);
        printf("%s\n", p3);
    }
    printf("OK\n\n");


    printf("=============== Lecture par élément d'une taille plus grande que le double du buffer système ===============\n");
    // Problème ici: p4 ne se termine pas à l'indice 16 avec '\0', et la dernière affectation à p3 ("preuve") semble se
    //               se concaténer à p4
    //               Remarque: n'affecte pas les tests suivants
    char p4[16] = {0};
    int ret4;

    for(int i = 0; i < 50; i++){
        ret4 = lire(p4, 16, 1, f);
        assert(ret4 == 1);
        // printf("%ld\n", strlen(p4));     // affiche 22..
        printf("%s\n\n", p4);
    }
    printf("OK\n\n");


    printf("=============== Lecture par élément d'une taille encore plus grande que le double du buffer système ===============\n");
    char p5[30] = {0};
    int ret5;

    for(int i = 0; i < 50; i++){
        ret5 = lire(p5, 30, 1, f);
        assert(ret5 == 1);
        printf("%s\n\n", p5);
    }
    printf("OK\n\n");


    printf("=============== Lecture de 2 éléments de 7 octets ===============\n");
    char p6[15] = {0};
    int ret6;

    for(int i = 0; i < 50; i++){
        ret6 = lire(p6, 7, 2, f);
        assert(ret6 == 2);
        printf("%s\n\n", p6);
    }
    printf("OK\n\n");

    fermer(f);

    return 0;
}
