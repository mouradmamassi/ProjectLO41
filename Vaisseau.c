#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#define NB_QUAIS_CHARGEMENT 3
#include "Drone.h"


Vaisseau* initVaisseau(int nbDrones, int nbColis){

    Vaisseau* v=malloc(sizeof(Vaisseau));
    v->dronesTab=malloc(sizeof(Drone*)*nbDrones);
    v->nbColisLivres=0;
    v->nbColisNonLivrables=0;
    v->nbColis=nbColis;
    v->nbDrones=nbDrones;
    sem_init(&v->garage[0], 0, 1);
    sem_init(&v->garage[1], 0, 1);
    sem_init(&v->finCharge, 0, NB_QUAIS_CHARGEMENT);
    sem_init(&v->fini,0,0);
    for(int i=0;i<NB_TYPES;i++){
        sem_init(&v->fileAttenteCharge[i], 0, 0);
    }

    v->queueGarage[0] = 0;
    v->queueGarage[1] = 0;
    v->garageOccupe = 0;
    pthread_mutex_init(&v->m[0], 0);
    pthread_mutex_init(&v->m[1], 0);
    pthread_mutex_init(&v->m[2], 0);

    nettoyer();
    for(int i = 0; i < NB_TYPES; i++){
        char c[7] = "/test";
        char d[2] = "0";
        d[0] = '0'+i;
        strcat(c, d);
        struct mq_attr* attr = NULL;
        struct mq_attr at;
        at.mq_msgsize = sizeof(Colis) + 4;
        at.mq_maxmsg = 10;
        at.mq_flags = O_RDWR;
        at.mq_curmsgs = 0;
        attr = &at;
        v->colisAttente[i] = mq_open(c, O_CREAT | O_RDWR, 0666, attr);
    }

    CreerAndPosterColis(v, nbColis);
    demarrerDrones(nbDrones,v);

    return v;
}

void nettoyer(){
    for(int i=0;i<NB_TYPES;i++){
        char c[7]="/test";
        char d[2]="0";
        d[0]='0'+i;
        strcat(c,d);
        mq_unlink(c);

    }
}



void demarrerDrones(int nb, Vaisseau* v){

    for(int i = 0; i < nb;i++){
        if( sizeof(v->colisAttente[i%NB_TYPES]) != NULL)
            creerDrone(i,i%NB_TYPES,v);
    }
}

void CreerAndPosterColis(Vaisseau* v, int nbColis){

    // DRONE MOYENNE  distance client < 30 car la charge = 90
    // DRONE GRANDE  distance client < 40 car la charge = 200
    // DRONE PETITE  distance client < 20 car la charge = 40


    for(int i = 0;i<nbColis;i++){
        Colis* c = (Colis*)malloc(sizeof(Colis));
        c->type = rand()%NB_TYPES;

//        if(c->type == PETITE){
//            c->client = rand()%20;
//        }

        switch(c->type){
            case PETITE:
                c->client = rand()%(AUTONOMIE_PETITE/3) + 1;
                break;
            case MOYENNE:
                c->client = rand()%(AUTONOMIE_MOYENNE/3) + 1;
                break;
            case GRANDE:
                c->client = rand()%(AUTONOMIE_GRANDE/3) + 1;
                break;
            default:
                break;
        }

        c->id = i;
        c->priorite = rand()%10;

        mq_send(v->colisAttente[c->type], (char*)c, sizeof(Colis) + 4, (unsigned int)c->priorite);
        printf("colis %d posté, type %d, distance %d, prio %d \n ", c->id,
               c->type, c->client, c->priorite);
    }


    sem_post(&v->fileAttenteCharge[GRANDE]);
    sem_post(&v->fileAttenteCharge[MOYENNE]);
    sem_post(&v->fileAttenteCharge[PETITE]);



}

void finOperation(Vaisseau* v){
    printf("\n\n \t------ Fin de l'operation ------\n\n");
    printf("Livraison échouée de %d colis\n",v->nbColis - v->nbColisLivres - v->nbColisNonLivrables);
    printf("Colis non livrables : %d\n",v->nbColisNonLivrables);
    printf("Colis livrés : %d\n\n",v->nbColisLivres);
    libererTout(v);
    nettoyer();
}


void libererTout(Vaisseau* v){
    for(int i=0;i<v->nbDrones;i++){
        detruireDrone(v->dronesTab[i]);
    }
    free(v->dronesTab);
    free(v);
}

void traitant(){
    finOperation(gv);
    exit(0);
}

