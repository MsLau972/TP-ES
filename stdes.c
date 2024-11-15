#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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
    void * buffer_ecriture = malloc(sizeof(char *) * BUFFER_SIZE);
    void * buffer_lecture = malloc(sizeof(char *) * BUFFER_SIZE);
    fichier->buffer_lecture=buffer_lecture;
    fichier->buffer_ecriture=buffer_ecriture;
    fichier->buffer_size=BUFFER_SIZE;
    fichier->mode=mode;
    fichier->fd=fd;
    fichier->curseur_lecture=0;
    fichier->curseur_ecriture=0;
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

    vider(f);// flush ici: le buffer peut ne pas être vide

    int c=close(f->fd);
    if(c<0){
        fprintf(stderr, "Erreur: Impossible de fermer le fichier");
        return -2;
    }

    free(f->buffer_lecture);
    free(f->buffer_ecriture);
    free(f);
    return 0;
}


/**
 *  lire - lit des éléments depuis le fichier pointé par f et les stocke dans le buffer utilisateur
 *  Paramètres: - p le buffer utilisateur
 *              - taille la taille d'un élement
 *              - nbelem le nombre d'éléments à lire
 *              - f un objet de type FICHIER
 * /!\ il est de la responsabilité de l'utilisateur de s'assurer que la taille de p est suffisante pour 
 * contenir l'ensemle des élèments à lire.
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
    size_t curseur_p=0;
    int nb_elem_lu=0;
    int nb_elem_buffer=(f->available_read-f->curseur_lecture)/taille;
    if(nb_elem_buffer>nbelem){
        nb_elem_buffer=nbelem;
    }
    memcpy((char *)p+curseur_p, f->buffer_lecture+f->curseur_lecture, nb_elem_buffer*taille);
    curseur_p+=nb_elem_buffer*taille;
    f->curseur_lecture+=nb_elem_buffer*taille;
    nb_elem_lu+=nb_elem_buffer;
    if(nb_elem_lu==nbelem){
        return nb_elem_buffer;
    }
    int octet_restant_buffer_lecture=f->available_read-f->curseur_lecture;
    memcpy((char *)p+curseur_p, f->buffer_lecture+f->curseur_lecture, octet_restant_buffer_lecture);
    curseur_p+=octet_restant_buffer_lecture;
    f->curseur_lecture+=octet_restant_buffer_lecture;
    int octet_restant_a_lire=(nbelem-nb_elem_lu)*taille-octet_restant_buffer_lecture;
    if(octet_restant_a_lire>f->buffer_size){
        int n=read(f->fd, (char *) p+curseur_p, octet_restant_a_lire);
        if(n==-1){
            //TODO
        }
        else{
            if((n+octet_restant_a_lire)%taille!=0){
                int octet_restant_elem_complet=taille-((n+octet_restant_a_lire)%taille);
                char vide [octet_restant_elem_complet];
                for(int i=0; i<octet_restant_elem_complet; i++){
                    vide[i]='\0';
                }
                memcpy((char *)p+curseur_p, vide, octet_restant_elem_complet);
                n+=octet_restant_elem_complet;
            }
            nb_elem_lu+=(n+octet_restant_buffer_lecture)/taille;
        }
        return nb_elem_lu;
    }

    int n=read(f->fd, f->buffer_lecture, f->buffer_size);
    f->available_read=n;
    f->curseur_lecture=0;
    if(octet_restant_a_lire>f->available_read){
        octet_restant_a_lire=f->available_read;
    }
    memcpy((char *)p+curseur_p, f->buffer_lecture+f->curseur_lecture, octet_restant_a_lire);
    curseur_p+=octet_restant_a_lire;
    f->curseur_lecture+=octet_restant_a_lire;
    // if((nbelem*taille)-octet_restant_a_lire>0){
    //     if(octet_restant_a_lire>f->buffer_size){
    //         int n=read(f->fd, (char *) p+curseur_p, octet_restant_a_lire);
    //         if(n==-1){
    //             //TODO
    //         }
    //         else{
    //             if((n+octet_restant_a_lire)%taille!=0){
    //                 int octet_restant_elem_complet=taille-((n+octet_restant_a_lire)%taille);
    //                 char vide [octet_restant_elem_complet];
    //                 for(int i=0; i<octet_restant_elem_complet; i++){
    //                     vide[i]='\0';
    //                 }
    //                 memcpy((char *)p+curseur_p, vide, octet_restant_elem_complet);
    //                 n+=octet_restant_elem_complet;
    //             }
    //             nb_elem_lu+=(n+octet_restant_a_lire)/taille;
    //         }
    //         return nb_elem_lu;
    //     }else{
    //         memcpy((char *)p+curseur_p, f->buffer_lecture+f->curseur_lecture, octet_restant_a_lire);
    //         curseur_p+=octet_restant_a_lire;
    //         f->curseur_lecture+=octet_restant_a_lire;
    //         octet_restant_a_lire=0;
    //     }
    // }
    if((n+octet_restant_a_lire)%taille!=0){
        int octet_restant_elem_complet=taille-((n+octet_restant_a_lire)%taille);
        char vide [octet_restant_elem_complet];
        for(int i=0; i<octet_restant_elem_complet; i++){
            vide[i]='\0';
        }
        memcpy((char *)p+curseur_p, vide, octet_restant_elem_complet);
        n+=octet_restant_elem_complet;

    }
    nb_elem_lu+=(octet_restant_buffer_lecture+octet_restant_a_lire)/taille;
    return nb_elem_lu;
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
    if(f->fd==-1){
        return -2;
    }
    if(f->mode!='E'){
        return -3;
    }

    if(taille*nbelem>f->buffer_size){
        if(f->curseur_ecriture>0){
            vider(f);
        }
        int n=write(f->fd, p, taille*nbelem);
        if(n%taille!=0){
            int octet_restant_elem_complet=taille-(n%taille);
                char vide [octet_restant_elem_complet];
                for(int i=0; i<octet_restant_elem_complet; i++){
                    vide[i]='\0';
                }
                write(f->fd, vide, octet_restant_elem_complet);
                n+=octet_restant_elem_complet;
        }
        return n/taille;
    }
    int nb_elem_buffer=(f->buffer_size-f->curseur_ecriture)/taille;
    if(nb_elem_buffer>=nbelem){
        memcpy(f->buffer_ecriture+f->curseur_ecriture, p, nbelem*taille);
        f->curseur_ecriture+=nbelem*taille;
        return nbelem;
    }
    memcpy(f->buffer_ecriture+f->curseur_ecriture, p, nb_elem_buffer*taille);
    f->curseur_ecriture+=nb_elem_buffer*taille;
    vider(f);
    int reste_a_ecrire=nbelem-nb_elem_buffer;
    memcpy(f->buffer_ecriture+f->curseur_ecriture, p, reste_a_ecrire*taille);
    f->curseur_ecriture+=reste_a_ecrire*taille;
    return nbelem;
}

// //     int reste = nbelem * taille;
// //     int total_ecriture=0;
// //     while(reste){
// //         //remplir le buffer
// //         if(reste>BUFFER_SIZE){ //cas où le buffer est entièrement rempli
// //             int ecrit = write(f->fd, f->buffer, BUFFER_SIZE);
// //             total_lecture+=ecrit;
// //             reste-=ecrit;
// //             p[total_ecriture+taille]=f->buffer[(f->curseur)-taille];
// //             f->curseur=0;
// //         }
// //         else{//cas où le buffer n'est pas entièrement rempli
// //             int ecrit = write(f->fd, f->buffer, reste);
// //             total_ecriture+=ecrit;
// //             reste-=ecrit;
// //             p[total_ecriture+taille]=f->buffer[(f->curseur)-taille];
// //         }
// //         if(!ecrit||ecrit<0){
// //             return -4;
// //         }

// //     }
// //     return total_ecriture/taille;
//  }


/**
    vider - écrit les données du buffer dans le fichier ouvert en écriture
        Paramètres: f un pointeur vers une structure FICHIER
        Valeur de retour: Retourne 0 si la fonction réussit, -1 sinon
 */
int vider(FICHIER *f){
    if(f->mode == 'E'){
        write(f->fd, f->buffer_ecriture, f->curseur_ecriture);
        f->curseur_ecriture=0;
        return 0;
    }

    return -1;
}

