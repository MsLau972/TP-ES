#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "stdes.h"

/* Test de lire() et ecrire() en même temps */

int main(){
    FICHIER * f_read =  ouvrir("fichiersTexte/Jekyll_et_Hyde.txt", 'L');
    assert(f_read != NULL);

    FICHIER * f_write = ouvrir("fichiersTexte/out_write3.txt", 'E');
    assert(f_write != NULL);

    char p[20];
    int nb_elem_read, nb_elem_wrote;

    // variables de comparaison pour la fin
    int max_length = 0;

    printf("=============== Lire BUFFER_SIZE + 100 caractères puis les écrire ===============\n");
    for(int i = 0; i < BUFFER_SIZE + 100; i++){
        nb_elem_read = lire(p, 1, 1, f_read);
        assert(nb_elem_read == 1);
        assert(f_read->curseur_lecture == i % BUFFER_SIZE + 1);

        nb_elem_wrote = ecrire(p, 1, 1, f_write);
        assert(nb_elem_wrote == 1);
        assert(f_write->curseur_ecriture == i % BUFFER_SIZE + 1);
        max_length += 1 * nb_elem_wrote;
    }
    // on a lu 100 caractères de plus que la taille des buffers systèmes
    assert(f_read->curseur_lecture == 100);
    assert(f_write->curseur_ecriture == 100);
    printf("OK\n");

    printf("=============== Lecture 5 éléments de 4 octets et écriture 4 éléments de 5 octets ===============\n");
    nb_elem_read = lire(p, 4, 5, f_read);
    assert(nb_elem_read == 5);
    assert(f_read->curseur_lecture == 120);

    nb_elem_wrote = ecrire(p, 5, 4, f_write);
    assert(nb_elem_wrote == 4);
    assert(f_write->curseur_ecriture == 120);
    max_length += 5 * nb_elem_wrote;
    printf("OK\n");

    printf("=============== Lecture 4 éléments de 5 octets et écriture 5 éléments de 3 octets ===============\n");
    nb_elem_read = lire(p, 5, 4, f_read);
    assert(nb_elem_read == 4);
    assert(f_read->curseur_lecture == 140);

    nb_elem_wrote = ecrire(p, 3, 5, f_write);
    assert(nb_elem_wrote == 5);
    assert(f_write->curseur_ecriture == 135);
    max_length += 3 * nb_elem_wrote;

    printf("OK\n");

    fermer(f_read);
    fermer(f_write);

    printf("=============== Vérification du contenu du fichier ===============\n");
    FILE *fd1 = fopen("fichiersTexte/out_write3.txt", "r");
    char content[max_length];
    fread(content, max_length, 1, fd1);

    FILE *fd2 = fopen("fichiersTexte/Jekyll_et_Hyde.txt", "r");
    char expected[max_length];
    fread(expected, max_length, 1, fd2);

    assert(strcmp(content, expected) == 0);
    printf("OK\n");

    fclose(fd1);
    fclose(fd2);

    return 0;
}
