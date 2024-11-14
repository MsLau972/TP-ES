#ifndef _STDES_H
#define _STDES_H
#include <stdint.h>
#define BUFFER_SIZE 1000

struct _ES_FICHIER{
    void * buffer;
    size_t buffer_size;
    char mode;
    int fd;
    size_t curseur;
    size_t available_read;  // données disponibles en lecture dans le buffer
};
typedef struct _ES_FICHIER FICHIER;

extern FICHIER *my_stdout;
extern FICHIER *my_stderr;

/* mode: 'L' = lecture, 'E' = écriture */
FICHIER *ouvrir(const char *nom, char mode);
int fermer(FICHIER*f);
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int vider(FICHIER *f);

int fecriref (FICHIER *f, const char *format, ...);
/* directly in stdout */
int ecriref (const char *format, ...);
int fliref (FICHIER *f, const char *format, ...);

#endif

