#include <stdio.h>
#include <stdlib.h>
#include "stdes.h"

int main(){
    FICHIER * f=ouvrir("fichiersTexte/Candide.txt", 'L');
    FICHIER * f2=ouvrir("fichiersTexte/out.txt", 'E');
    char p [50];
    f->buffer_size=20;
    int n = lire(p, 5, 3, f);
    printf("n= %d\n", n);
    printf("p= %s\n", p);
    printf("b= %s\n", (char *)f->buffer_lecture);
    int n2= ecrire(p, 1, 2, f2);
    printf("n2= %d\n", n2);
    printf("s= %s\n", (char *)f2->buffer_ecriture);
    fermer(f);
    return 0;
}