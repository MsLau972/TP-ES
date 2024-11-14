#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "stdes.h"

/**
 *  ouvrir - ouvre un fichier dont le nom et le mode d'ouverture sont passés en paramètre
 *  Paramètres: - nom le nom du fichier
 *              - mode le mode d'ouverture du fichier
 *  Valeur de retour: un pointeur sur un objet de type FICHIER, NULL si le fichier n'a pas pu être ouvert
*/
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
        fprintf(stderr, "Erreur: Fichier introuvable");
        return NULL;
    }

    FICHIER* fichier = (FICHIER*)(malloc(sizeof(FICHIER)));
    void * buffer = malloc(sizeof(char *) * BUFFER_SIZE);
    fichier->buffer=buffer;
    fichier->buffer_size=BUFFER_SIZE;
    fichier->mode=mode;
    fichier->fd=fd;
    fichier->curseur=0;
    fichier->available_read=0; // buffer vide au début, donc rien ne peut être lu

    return fichier;
}


/**
 *  fermer - ferme le fichier pointé par f
 *  Paramètres: f un objet de type FICHIER
 *  Valeur de retour: Retourne 0 si la fermeture a réussi, -1 si f n'existe pas et -2 si la fermeture du fichier a échoué
*/
int fermer(FICHIER*f){
    if (f==NULL){
        fprintf(stderr, "Erreur: La structure f n'existe pas");
        return -1;
    }

    // flush ici: le buffer peut ne pas être vide

    int c=close(f->fd);
    if(c<0){
        fprintf(stderr, "Erreur: Impossible de fermer le fichier");
        return -2;
    }

    free(f->buffer);
    free(f);
    return 0;
}


/**
 *  lire - lit des éléments depuis le fichier pointé par f et les stocke dans le buffer utilisateur
 *  Paramètres: - p le buffer utilisateur
 *              - taille la taille d'un élement
 *              - nbelem le nombre d'éléments à lire
 *              - f un objet de type FICHIER
 *  Valeur de retour: TODO
*/
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
    
    // TODO: refaire: l'objectif est d'appeler read() uniquement en cas de besoin

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


/**
 *  ecrire - écrit des éléments stockés dans le buffer utilisateur dans le fichier pointé par f
 *  Paramètres: - p le buffer utilisateur
 *              - taille la taille d'un élement
 *              - nbelem le nombre d'éléments à lire
 *              - f un objet de type FICHIER
 *  Valeur de retour: TODO
*/
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


/**
    vider - écrit les données du buffer dans le fichier ouvert en écriture
        Paramètres: f un pointeur vers une structure FICHIER
        Valeur de retour: Retourne 0 si la fonction réussit, -1 sinon
 */
int vider(FICHIER *f){
    if(f->mode == 'E'){
        write(f->fd, f->buffer, f->curseur);
        f->curseur=0;
        return 0;
    }

    return -1;
}
