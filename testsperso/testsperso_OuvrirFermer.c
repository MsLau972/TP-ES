#include <stdio.h>
#include <stdlib.h>
#include "stdes.h"

int main(){
    FICHIER* f;
    f=ouvrir("fff", 'L'); //ouvre et créer un fifhier inexistant
    int close=fermer(f);// ferme le fichier
    printf("close = %d\n", close);
    return 0;
}