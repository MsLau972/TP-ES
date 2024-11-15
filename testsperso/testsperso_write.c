#include <stdio.h>
#include <stdlib.h>
#include "stdes.h"

int main(){
    FICHIER * f=ouvrir("fichiersTexte/out.txt", 'E');
    char p [10]= "0123456789";
    char p2 [10]= "azertyuiop";
    f->buffer_size=8;
    int n= ecrire(p, 4, 2, f);
    printf("n= %d\n", n);
    printf("s= %s\n", (char *)f->buffer_ecriture);
    int n2= ecrire(p2, 1, 2, f);
    printf("n= %d\n", n2);
    printf("s= %s\n", (char *)f->buffer_ecriture);
    fermer(f);
    return 0;
}
