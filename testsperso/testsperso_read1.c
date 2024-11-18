#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stdes.h"

// tests faciles pour la fonction lire()

int main(){
    FICHIER * f=ouvrir("fichiersTexte/Candide.txt", 'L');   // fichier long
    char p[50];
    f->buffer_size=20;

    printf("=============== Lecture de 3 éléments de 5 octets (multiple de 20) ===============\n");
    int n = lire(p, 5, 3, f);   // 3 éléments de 5 octets
    assert(n == 3);
    printf("p= %s\n", p);
    printf("buffer_lecture= %s\n", (char *)f->buffer_lecture);
    printf("OK\n\n");


    printf("=============== Lecture de 1 élément de 6 octets (non multiple de 20) ===============\n");
    int n2=lire(p, 6, 1, f);    // 1 élément de 6 octets
    assert(n2 == 1);    
    printf("p= %s\n", p);
    printf("buffer_lecture= %s\n", (char *)f->buffer_lecture);
    printf("OK\n\n");

    fermer(f);
    return 0;
}
