#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>

#include "stdes.h"

// tests pour la fonction lire() sur un fichier très court qui contient "Hello World !" (13 caractères)

int main(){

    printf("=============== Lecture de 1 élément de 13 octets ===============\n");
    FICHIER * f13 =ouvrir("fichiersTexte/texte_court.txt", 'L');  
    f13->buffer_size=8;
    char chaine13[13];

    int ret13 = lire(chaine13, 13, 1, f13);
    assert(ret13 == 1);
    printf("%s\n", chaine13);
    printf("OK\n");
    printf("Fermeture du fichier...\n\n");
    fermer(f13);


    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("=============== Lecture de 1 élément de 14 octets ===============\n");
    FICHIER * f14 =ouvrir("fichiersTexte/texte_court.txt", 'L');  
    f14->buffer_size=8;
    char chaine14[14] = {0};
    // printf("%p\n", &chaine14[0]);

    int ret14 = lire(chaine14, 14, 1, f14);
    assert(ret14 == 0);
    printf("%s\n", chaine14);
    printf("OK\n");
    printf("Fermeture du fichier...\n\n");

    fermer(f14);


    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("=============== Lecture de 1 élément de 7 octets ===============\n");
    // même problème ici que dans testsperso_read2
    FICHIER * f7_1 =ouvrir("fichiersTexte/texte_court.txt", 'L');  
    f7_1->buffer_size=8;
    char chaine7_1[7];
    // printf("%p\n", &chaine7_1[7]);       // on a &chaine7_1[7] == &chaine14[0] ... Et ce avant même l'appel à lire()
    
    int ret7_1 = lire(chaine7_1, 7, 1, f7_1);
    assert(ret7_1 == 1);
    printf("%s\n", chaine7_1);
    printf("OK\n");

    printf("=============== Lecture de 1 élément de 7 octets ===============\n");
    // (il n'y a pas assez pour faire un élément entier)
    char chaine7_2[7];
    // printf("%p\n", &chaine7_2[0]);

    int ret7_2 = lire(chaine7_2, 7, 1, f7_1);
    assert(ret7_2 == 0);
    assert(strlen(chaine7_2) > 0);
    printf("%s\n", chaine7_2);
    printf("OK\n");
    printf("Fermeture du fichier...\n\n");

    fermer(f7_1);


    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("=============== Lecture de 1 élément de 13 octets ===============\n");
    FICHIER * f_entier =ouvrir("fichiersTexte/texte_court.txt", 'L');  
    f_entier->buffer_size=8;
    char chaine_entier[13];     
    // printf("%p\n", &chaine_entier[13]);      // on a &chaine_entier[13] == &chaine7_2[0]
    
    int ret_entier = lire(chaine_entier, 13, 1, f_entier);
    assert(ret_entier == 1);
    printf("%s\n", chaine_entier);
    printf("OK\n");

    printf("=============== Lecture de 1 élément de 3 octets ===============\n");
    // (on veut lire un élément mais il n'y a plus rien à lire dans le fichier)
    char chaine3[3];

    int ret3 = lire(chaine3, 3, 1, f_entier);
    assert(ret3 == 0);
    assert(strlen(chaine3) == 0);
    printf("%s\n", chaine3);
    printf("OK\n");
    printf("Fermeture du fichier...\n\n");

    fermer(f_entier);


    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("=============== Lecture de 1 élément de 12 octets ===============\n");
    FICHIER * f1_12 =ouvrir("fichiersTexte/texte_court.txt", 'L');  
    f1_12->buffer_size=30;
    char chaine1_12[12];
    
    int ret1_12 = lire(chaine1_12, 12, 1, f1_12);
    assert(ret1_12 == 1);
    assert(strlen(chaine1_12) > 0);
    printf("%s\n", chaine1_12);
    printf("OK\n");

    printf("=============== Lecture de 1 élément de 3 octets ===============\n");
    // (on veut lire un élément mais il n'y a plus rien à lire dans le fichier (mais il reste quelques octets dans le buffer))
    char chaine1_3[3];

    int ret1_3 = lire(chaine1_3, 3, 1, f1_12);
    assert(ret1_3 == 0);
    assert(strlen(chaine1_3) > 0);
    printf("%s\n", chaine1_3);
    printf("OK\n");
    printf("Fermeture du fichier...\n\n");

    fermer(f1_12);


    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("=============== Lecture de 2 élément de 10 octets ===============\n");
    // (plus que le buffer mais ce qu'on lit au total (13) n'est pas multiple de 10)
    FICHIER * f2_10 =ouvrir("fichiersTexte/texte_court.txt", 'L');  
    f2_10->buffer_size = 9;
    char chaine2_10[20];
    
    int ret2_10 = lire(chaine2_10, 10, 2, f2_10);
    assert(ret2_10 == 1);
    assert(strlen(chaine2_10) > 0);
    printf("%s\n", chaine2_10);
    printf("OK\n");
    printf("Fermeture du fichier...\n\n");

    fermer(f2_10);

    return 0;
}
