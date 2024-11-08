#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "stdes.h"

FICHIER *ouvrir(const char *nom, char mode){
    int flags;
    if (mode =='E'){
        flags= O_WRONLY | O_CREAT;
    }else if(mode =='L'){
        flags=O_RDONLY | O_CREAT;
    }
    else{
        return NULL;
    }
    int fd=open(nom, flags, mode);
    if(fd<0){
        return NULL;
    }
    FICHIER* fichier = (FICHIER*)(malloc(sizeof(FICHIER)));
    void * buffer = malloc(BUFFER_SIZE);
    fichier->buffer=buffer;
    fichier->buffer_size=BUFFER_SIZE;
    fichier->mode=mode;
    fichier->fd=fd;
    fichier->curseur=0;
    fichier->available_read=BUFFER_SIZE;

    return fichier;
}

int fermer(FICHIER*f){
    if (f==NULL){
        return -1;
    }
    int c=close(f->fd);
    if(c<0){
        return -2;
    }
    free(f->buffer);
    free(f);
    return 0;
}

int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    if(f==NULL){
        return -1;
    }
    if(!f->fd){
        return -2;
    }
    if(f->mode!='L'){
        return -3;
    }
    if((f->curseur)=0){
        int lu=read(f->fd, f->buffer, BUFFER_SIZE);
        p[lu]=f->buffer[taille];
    }
    int reste = nbelem * taille;
    int total_lecture=0;
    while(reste){
        //remplir le buffer
        if(reste>BUFFER_SIZE){ //cas où le buffer est entièrement rempli
            int lu = read(f->fd, f->buffer, BUFFER_SIZE);
            total_lecture+=lu;
            reste-=lu;
            p[total_lecture+taille]=f->buffer[(f->curseur)-taille];
            f->curseur=0;
        }
        else{//cas où le buffer n'est pas entièrement rempli
            int lu = read(f->fd, f->buffer, reste);
            total_lecture+=lu;
            reste-=lu;
            p[total_lecture+taille]=f->buffer[(f->curseur)-taille];
        }
        if(!lu||lu<0){
            return -4;
        }

    }
    
    return total_lecture/taille;
}

int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    if(f==NULL){
        return -1;
    }
    if(f->fd==NULL){
        return -2;
    }
    if(f->mode!='E'){
        return -3;
    }
    int reste = nbelem * taille;
    int total_ecriture=0;
    while(reste){
        //remplir le buffer
        if(reste>BUFFER_SIZE){ //cas où le buffer est entièrement rempli
            int ecrit = write(f->fd, f->buffer, BUFFER_SIZE);
            total_lecture+=ecrit;
            reste-=ecrit;
            p[total_ecriture+taille]=f->buffer[(f->curseur)-taille];
            f->curseur=0;
        }
        else{//cas où le buffer n'est pas entièrement rempli
            int ecrit = write(f->fd, f->buffer, reste);
            total_ecriture+=ecrit;
            reste-=ecrit;
            p[total_ecriture+taille]=f->buffer[(f->curseur)-taille];
        }
        if(!ecrit||ecrit<0){
            return -4;
        }

    }
    return total_ecriture/taille;
}
int vider(FICHIER *f){
    write(f->fd, f->buffer, f->curseur);
    f->curseur=0;
    return 0;
}
