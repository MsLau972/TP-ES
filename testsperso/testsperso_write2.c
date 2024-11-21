#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>

#include "stdes.h"

#define BUFFER_SIZE_BIS 6

/* 
   Fichier pour tester essentiellement la fonction ecrire sur un buffer de taille petite (ici 6). 
   On devrait avoir a la fin toutes les chaines de caracteres ecrites les unes a la suite des autres
   (sans espace entre eux) dans le fichier out.txt
*/

int main(int argc, char *argv[]){
    FICHIER *f;
    char mem[14];

    // variables pour la comparaison de fin
    char expected[100];
    size_t max_length = 0;

    // Ouverture d'un fichier correct
    f = ouvrir("fichiersTexte/out.txt", 'E');
    assert(f != NULL);

    // Modification de la taille du buffer
    f->buffer_size = BUFFER_SIZE_BIS;

    printf("=============== Test: taille donnees inferieure a celle du buffer et buffer vide ===============\n");
    strcpy(mem, "Fluo");
    ssize_t nbElems = ecrire((void *)mem, strlen(mem), 1, f);
    assert(nbElems == 1);
    assert(f->curseur_ecriture == strlen(mem));
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: taille donnees plus grande que buffer et buffer non vide ===============\n");
    strcpy(mem, "Bonjour!");
    nbElems = ecrire((void *)mem, strlen(mem), 1, f);
    assert(nbElems == 1);
    assert(f->curseur_ecriture == 0);
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: taille donnees plus grande que buffer et buffer vide ===============\n");
    strcpy(mem, "Bonjour!");
    nbElems = ecrire((void *)mem, strlen(mem), 1, f);
    assert(nbElems == 1);
    assert(f->curseur_ecriture == 0);
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: 2 elements de taille 3 ===============\n");
    strcpy(mem, "photos");
    nbElems = ecrire((void *)mem, 3, 2, f);
    assert(nbElems == 2);
    assert(f->curseur_ecriture == strlen(mem));
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: 2 elements de taille 2 ===============\n");
    strcpy(mem, "bleu");
    nbElems = ecrire((void *)mem, 2, 2, f);
    assert(nbElems == 2);
    assert(f->curseur_ecriture == strlen(mem));
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: 2 elements de taille 4 ===============\n");
    strcpy(mem, "Pochette");
    nbElems = ecrire((void *)mem, 4, 2, f);
    assert(nbElems == 2);
    assert(f->curseur_ecriture == 0);   // a nécessité un flush
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: 3 elements de taille 1 ===============\n");
    strcpy(mem, "Car");
    nbElems = ecrire((void *)mem, 1, 3, f);
    assert(nbElems == 3);
    assert(f->curseur_ecriture == 3);
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");

    printf("=============== Test: donnees de taille au moins 2*BUFFER_SIZE_BIS ===============\n");
    strcpy(mem, "Hello World!");
    nbElems = ecrire((void *)mem, strlen(mem), 1, f);
    assert(nbElems == 1);
    assert(f->curseur_ecriture == 0);
    max_length += strlen(mem);
    strcat(expected, mem);
    printf("OK\n");
    
    fermer(f);

    printf("=============== Vérification du contenu du fichier ===============\n");
    FILE *fd = fopen("fichiersTexte/out.txt", "r");
    char content[max_length];
    fread(content, max_length, 1,  fd);

    assert(strcmp(content, expected) == 0);
    printf("OK\n");

    fclose(fd);

    return 0;
}
