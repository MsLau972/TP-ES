#include <stdio.h>
#include <stdlib.h>
#include "stdes.h"
#include <assert.h>
#include <unistd.h>

// tests pour l'ouverture et la fermeture d'un fichier + robustesse

int main(){
    FICHIER* f = NULL;
    int ret;
    int close;

    printf("=============== Ouverture en lecture d'un fichier ===============\n");
    f=ouvrir("fff", 'L'); //ouvre et créé un fichier inexistant
    assert(f != NULL);
    assert(f->buffer_ecriture != NULL);
    assert(f->buffer_lecture != NULL);
    assert(f->buffer_size == BUFFER_SIZE);
    assert(f->mode == 'L');
    assert(f->curseur_ecriture == 0);
    assert(f->curseur_lecture == 0);
    assert(f->available_read == 0);

    // on vérifie qu'on ne peut rien écrire dans le fichier
    ret = write(f->fd, "Hello World !", 5);     
    assert(ret == -1);

    printf("OK\n\n");


    printf("=============== Fermeture correct d'un fichier ===============\n");
    close=fermer(f);// ferme le fichier
    assert(close == 0);
    printf("OK\n\n");


    printf("=============== Ouverture en écriture d'un fichier ===============\n");
    f=ouvrir("fff", 'E'); //ouvre et créé un fichier inexistant
    assert(f != NULL);
    assert(f->buffer_ecriture != NULL);
    assert(f->buffer_lecture != NULL);
    assert(f->buffer_size == BUFFER_SIZE);
    assert(f->mode == 'E');
    assert(f->curseur_ecriture == 0);
    assert(f->curseur_lecture == 0);
    assert(f->available_read == 0);

    // on vérifie qu'on ne peut rien lire depuis le fichier
    ret = read(f->fd, "Hello World !", 5);     
    assert(ret == -1);
    fermer(f);
    printf("OK\n\n");


    printf("=============== Ouverture d'un fichier avec mode inexistant ===============\n");
    f=ouvrir("fff", 'R');
    assert(f == NULL);
    printf("OK\n\n");


    printf("=============== Ouverture d'un fichier avec mode inexistant ===============\n");
    f=ouvrir("fff", 'W');
    assert(f == NULL);
    printf("OK\n\n");

    return 0;
}