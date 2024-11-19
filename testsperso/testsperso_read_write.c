#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>

#include "stdes.h"

/*  Copier le contenu du fichier exécutable ./bin/hello_world dans ./bin/hello_world_copy 
    (normalement ./bin/hello_world_copy peut être exécuter)
*/

int main() {
    FICHIER *f_read = ouvrir("./bin/hello_world", 'L');
    assert(f_read != NULL);

    FICHIER *f_write = ouvrir("./bin/hello_world_copy", 'E');
    assert(f_write != NULL);

    char p[100];
    int nb_elem_read;

    while ((nb_elem_read = lire(p, 1, sizeof(p), f_read)) > 0) {
        ecrire(p, 1, nb_elem_read, f_write);
    }

    // Reproduire les permissions d'exécution
    struct stat file_stat;
    if (stat("./bin/hello_world", &file_stat) == 0) {
        chmod("./bin/hello_world_copy", file_stat.st_mode);
    } else {
        fprintf(stderr, "Erreur: Impossible de donner les mêmes permissions à ./bin/hello_world_copy\n");
    }

    fermer(f_read);
    fermer(f_write);

    return 0;
}
