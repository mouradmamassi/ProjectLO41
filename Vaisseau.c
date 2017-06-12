
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "Drone.h"

    Vaisseau* initVaisseau(void){
        Vaisseau* v=malloc(sizeof(Vaisseau));
        sem_init(&v->garage[0],0,1);
        sem_init(&v->garage[1],0,1);
        sem_init(&v->finCharge,0,1);
        for(int i=0;i<NB_TYPES;i++){
            sem_init(&v->fileAttenteCharge[i],0,0);
        }
        v->queueGarage[0]=0;
        v->queueGarage[1]=0;
        v->garageOccupe=0;
        pthread_mutex_init(&v->m[0],0);
        pthread_mutex_init(&v->m[1],0);
        pthread_mutex_init(&v->m[2],0);
        return v;
    }

    void demarrerDrones(int nb,Vaisseau* v){
        int j, i = 0;
        Colis *c = malloc(sizeof(Colis));
        for(i=0;i<NB_TYPES;i++){
            for(j=0;j<nb;j++){
                *c = getColis(v);
                if(c->priorite != 0)
                    creerDrone(i*(NB_TYPES-1)+j,c->type,v, c);
            }
        }
    }

    void posterColis(Vaisseau* v){

        Colis* coliss = (Colis*) malloc(sizeof(Colis) * NB_COLIS);
        coliss[0] = *creerColis(GRANDE, 1, 20, 3, ENCOURS);
        coliss[1] = *creerColis(MOYENNE, 2, 2, 4, ENCOURS);
        coliss[2] = *creerColis(GRANDE, 3, 2, 3, ENCOURS);
        coliss[3] = *creerColis(MOYENNE, 4, 2, 4, ENCOURS);
        coliss[4] = *creerColis(GRANDE, 5, 2, 3, ENCOURS);
        coliss[5] = *creerColis(PETITE, 6, 2, 4, ENCOURS);

        for(int i = 0; i < NB_COLIS; i++){
            printf("J'ai un colis de type %d\n",coliss[i].type);
            sem_post(&v->fileAttenteCharge[coliss[i].type]);
        }
         v->c = coliss;
    }
