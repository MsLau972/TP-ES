#include <stdio.h>
#include <stdlib.h>
#include "stdes.h"

int main(){
    FICHIER * f=ouvrir("fichiersTexte/Candide.txt", 'L');
    char p [50];
    f->buffer_size=20;
    int n = lire(p, 5, 3, f);
    printf("n= %d\n", n);
    printf("p= %s\n", p);
    printf("b= %s\n", (char *)f->buffer_lecture);
    int n2=lire(p, 6, 1, f);
    printf("n2= %d\n", n2);
    printf("b= %s\n", (char *)f->buffer_lecture);
    printf("p= %s\n", p);
    fermer(f);
    return 0;
}
