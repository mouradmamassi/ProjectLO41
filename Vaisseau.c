
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "Drone.h"

    Vaisseau* initVaisseau(void){
        Vaisseau* v=malloc(sizeof(Vaisseau));
        sem_init(&v->garage[0],0,1);
        sem_init(&v->garage[1],0,1);
        sem_init(&v->finCharge,0,3);
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
        for(int i=0;i<NB_TYPES;i++){
            for(int j=0;j<nb;j++){
                creerDrone(i*(NB_TYPES-1)+j,i,v);
            }
        }
    }

    int creerColis(void){
        return rand() % 3;
    }

    void posterColis(Vaisseau* v){
        int colis=creerColis();
        printf("J'ai un colis de type %d\n",colis);
        sem_post(&v->fileAttenteCharge[colis]);
    }
