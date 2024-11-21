#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FICHIER *f1;
	FICHIER *f2;
	char c;

	if (argc != 3){
		fprintf(stderr, "Usage: ./bin/test <fichier lecture> <fichier écriture>\n");
		exit(-1);
	}

	f1 = ouvrir (argv[1], 'L');
	if (f1 == NULL)
		exit (-1);

	f2 = ouvrir (argv[2], 'E');
	if (f2 == NULL)
		exit (-1);

	char erreur[] = "Erreur: Impossible de lire depuis le fichier f2\n";
	int ret = lire(&c, 1, 1, f2);
	if(ret < 0){
		ecrire(erreur, strlen(erreur), 1, my_stderr);
	}

	while (lire (&c, 1, 1, f1) == 1) {
        ecrire (&c, 1, 1, my_stdout);
        ecrire (&c, 1, 1, f2);
	}
	// vider (my_stdout);

    fermer (f1);
    fermer (f2);

	return 0;
}
