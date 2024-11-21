#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#include "stdes.h"

FICHIER * my_stdout = NULL;
FICHIER * my_stderr = NULL;

void before_main() __attribute__((constructor));
void after_main() __attribute__((destructor));

/**
 *  before_main - appelée avant une fonction main(), elle initialise my_stdout et my_stderr
*/
void before_main(){
	my_stdout = (FICHIER *)malloc(sizeof(FICHIER));
	my_stdout->buffer_lecture = NULL;
	my_stdout->buffer_ecriture = malloc(sizeof(char) * BUFFER_SIZE);
	my_stdout->buffer_size = BUFFER_SIZE;
	my_stdout->mode = 'E';
	my_stdout->fd = STDOUT_FILENO;
	my_stdout->curseur_lecture = 0;
    my_stdout->curseur_ecriture = 0;
    my_stdout->available_read = 0;

	my_stderr = (FICHIER *)malloc(sizeof(FICHIER));
    my_stderr->buffer_lecture = NULL; 
    my_stderr->buffer_ecriture = malloc(sizeof(char) * BUFFER_SIZE);
    my_stderr->buffer_size = BUFFER_SIZE;
    my_stderr->mode = 'E'; 
    my_stderr->fd = STDERR_FILENO;
    my_stderr->curseur_lecture = 0; 
    my_stderr->curseur_ecriture = 0;
    my_stderr->available_read = 0; 
}


/**
 *  after_main - appelée après une fonction main(), elle ferme my_stdout et my_stderr
*/
void after_main(){
	fermer(my_stderr);
	fermer(my_stdout);
}


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

    int fd=open(nom, flags, S_IRUSR | S_IWUSR);
    if(fd<0){
        fprintf(stderr, "Erreur: Fichier introuvable");
        return NULL;
    }

    FICHIER* fichier = (FICHIER*)(malloc(sizeof(FICHIER)));
    fichier->buffer_lecture=malloc(sizeof(char) * BUFFER_SIZE);;
    fichier->buffer_ecriture=malloc(sizeof(char) * BUFFER_SIZE);;
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

    if(f->curseur_ecriture > 0){
        vider(f);// flush ici: le buffer peut ne pas être vide
    }

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
 *  copy_to_usr_buf - copie les données du buffer système vers le buffer utilisateur
 *  Paramètres: - p le buffer utilisateur
 *              - curseur_p l'endroit dans le buffer où il faut ajouter des données
 *              - size_to_copy le nombre d'octets qu'il faut copier
 *              - f le fichier contenant le buffer système
*/
void copy_to_usr_buf(void *p, size_t *curseur_p, int size_to_copy, FICHIER *f){
    if(size_to_copy > 0){
        memcpy((char *)p + *curseur_p, f->buffer_lecture + f->curseur_lecture, size_to_copy);
        *curseur_p += size_to_copy;
        f->curseur_lecture += size_to_copy;
    }
}


/**
 *  lire - lit des éléments depuis le fichier pointé par f et les stocke dans le buffer utilisateur
 *  Paramètres: - p le buffer utilisateur
 *              - taille la taille d'un élement
 *              - nbelem le nombre d'éléments à lire
 *              - f un objet de type FICHIER
 * /!\ il est de la responsabilité de l'utilisateur de s'assurer que la taille de p est suffisante pour 
 * contenir l'ensemble des éléments à lire.
 *  Valeur de retour: Retourne le nombre d'éléments lus, -1 si le fichier n'existe pas et -2 si le fichier n'est
 *                    pas ouvert en lecture
*/
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    if(f==NULL || f->fd < 0){
        return -1;
    }
    if(f->mode!='L'){
        return -2;
    }
    size_t curseur_p=0;
    int nb_elem_lu=0;

    // on regarde combien d'éléments on peut mettre au total dans le buffer de lecture
    int nb_elem_buffer=(f->available_read-f->curseur_lecture)/taille;
    if(nb_elem_buffer>nbelem){
        // on prend uniquement ce qu'on veut lire
        nb_elem_buffer=nbelem;
    }

    copy_to_usr_buf(p, &curseur_p, nb_elem_buffer * taille, f);
    nb_elem_lu+=nb_elem_buffer;

    // on a lu tout ce qu'on voulait lire, donc on retourne
    if(nb_elem_lu==nbelem){
        return nb_elem_lu;
    }

    // si on est ici, c'est qu'on n'a pas lu tout ce qu'on voulait lire

    // on lit d'abord les données, en octets, qu'il reste dans le buffer de lecture
    // (donc pas forcément un élément en entier)
    int octet_restant_buffer_lecture=f->available_read-f->curseur_lecture;
    copy_to_usr_buf(p, &curseur_p, octet_restant_buffer_lecture, f);

    // on calcule ce qu'il nous reste à lire en octets
    int octet_restant_a_lire=(nbelem-nb_elem_lu)*taille-octet_restant_buffer_lecture;

    // si ce qu'il reste à lire est plus grand que la taille du buffer, alors
    // on bypass (ie on lit directement depuis le fichier)
    if(octet_restant_a_lire>f->buffer_size){
        int n=read(f->fd, (char *) p+curseur_p, octet_restant_a_lire);
        if(n==-1){
            fprintf(stderr, "Erreur: la lecture depuis le fichier a échoué\n");
        }
        else{
            nb_elem_lu+=(n+octet_restant_buffer_lecture)/taille;
        }

        return nb_elem_lu;
    }

    // ce qu'il reste à lire est suffisamment petit pour être contenu dans le buffer
    // donc pas besoin de bypass

    // on a lu toutes les données du buffer, il faut donc le re-remplir
    int n=read(f->fd, f->buffer_lecture, f->buffer_size);
    if(n == -1){
        fprintf(stderr, "Erreur; Impossible de lire depuis le fichier\n");
        return nb_elem_lu;
    }
    f->available_read=n;
    f->curseur_lecture=0;
    
    // on ne lit pas plus que ce qu'il y a dans le buffer
    // (typiquement on a un EOF)
    if(octet_restant_a_lire>f->available_read){
        octet_restant_a_lire=f->available_read;
    }
    copy_to_usr_buf(p, &curseur_p, octet_restant_a_lire, f);
    nb_elem_lu+=(octet_restant_buffer_lecture+octet_restant_a_lire)/taille;
    
    return nb_elem_lu;
}
                


/**
 *  ecrire - écrit des éléments stockés dans le buffer utilisateur dans le fichier pointé par f
 *  Paramètres: - p le buffer utilisateur
 *              - taille la taille d'un élement
 *              - nbelem le nombre d'éléments à lire
 *              - f un objet de type FICHIER
 *  /!\ L'utilisateur a la responsabilité de s'assurer que p contient au moins taille * nbelem éléments
 *  Valeur de retour: Retourne le nombre d'éléments qui ont été écrits, -1 si le fichier n'existe pas et -2 si le fichier
 *                    n'est pas ouvert en écriture
*/
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    if(f==NULL || f->fd == -1){
        return -1;
    }
    if(f->mode!='E'){
        return -2;
    }

    int bytes_to_read = taille * nbelem;

    // Si la taille de ce qu'il faut lire est plus grande que le buffer
    if(bytes_to_read>f->buffer_size){
        // flush s'il y a des données dans le buffer
        if(f->curseur_ecriture>0){
            vider(f);
        }

        // on bypass
        int n=write(f->fd, p, bytes_to_read);
        if(n == -1){
            fprintf(stderr, "Erreur: L'écriture dans le fichier a échoué\n");
            return 0;
        }

        // ce qui a été écrit n'est pas multiple de taille
        if(n%taille!=0){
            int octet_restant_elem_complet=taille-(n%taille);

            char vide[octet_restant_elem_complet];
            for(int i=0; i<octet_restant_elem_complet; i++){
                vide[i]='\0';
            }

            write(f->fd, vide, octet_restant_elem_complet);
            n+=octet_restant_elem_complet;
        }

        return n/taille;
    }

    // pas de bypass

    // on regarde combien d'éléments au maximum on peut mettre dans le buffer
    int nb_elem_max_in_buffer=(f->buffer_size-f->curseur_ecriture)/taille;

    // Il y a suffisamment de place pour tout contenir
    if(nb_elem_max_in_buffer>=nbelem){
        memcpy(f->buffer_ecriture+f->curseur_ecriture, p, bytes_to_read);
        f->curseur_ecriture+=bytes_to_read;
        return nbelem;
    }

    // Il n'y a pas assez de place pour tout contenir

    // On met dans le buffer ce qu'on peut
    memcpy(f->buffer_ecriture+f->curseur_ecriture, p, nb_elem_max_in_buffer*taille);
    f->curseur_ecriture+=nb_elem_max_in_buffer*taille;

    vider(f);

    int nb_elem_to_read=nbelem-nb_elem_max_in_buffer;
    memcpy(f->buffer_ecriture+f->curseur_ecriture, p, nb_elem_to_read*taille);
    f->curseur_ecriture+=nb_elem_to_read*taille;

    return nbelem;
}


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

