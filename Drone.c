

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "Drone.h"


    int creerDrone(int id, int type,Vaisseau* v){
        pthread_t th;
        Drone *ceDrone=malloc(sizeof(Drone));
        ceDrone->id=id;
        ceDrone->charge=AUTONOMIE;
        ceDrone->type=type;
        ceDrone->v=v;
        if (pthread_create(&th, 0, actionDrone, (void *) ceDrone) != 0)
            erreur("Erreur Creation thread");
        return (int)th;

    }

    void erreur(const char *msg)
    {
        perror(msg);
        exit(1);
    }

    void aller(Drone* d,int distance){
        for(int i=0;i<distance;i++){
            sleep(1);
            d->charge--;
            if(d->charge<0){
                printf("C'est le drone %d de type %d, je me crash\n",d->id,d->type);
                pthread_exit(NULL);
            }
        }
    }

    void* actionDrone(void* data){
        Drone* d=(Drone*)data;
        while(1){
            retourVaisseau(d);
            preparationLivraison(d);
            livraison(d);
        }
        pthread_exit(0);
    };

    void livraison(Drone* d){
        int ou=rand()%15;
        aller(d,ou);
        printf("C'est le drone %d de type %d, je suis chez le client\n",d->id,d->type);
        aller(d,ou);
    }

    void retourVaisseau(Drone*  d){
        sem_wait(&d->v->garage);
        sem_post(&d->v->garage);
        printf("C'est le drone %d de type %d, je suis rentré, je recharge\n",d->id,d->type);
        sleep(AUTONOMIE-d->charge);
        d->charge=AUTONOMIE;
        printf("C'est le drone %d de type %d, je suis rechargé, j'attends\n",d->id,d->type);
    }

    void preparationLivraison(Drone* d){
        sem_wait(&d->v->fileAttenteCharge[d->type]);
        printf("C'est le drone %d de type %d, j'attends pour charger\n",d->id,d->type);
        sem_wait(&d->v->finCharge);
        sem_wait(&d->v->garage);
        sem_post(&d->v->garage);
        printf("C'est le drone %d de type %d, je charge\n",d->id,d->type);
        sleep(1);
        printf("C'est le drone %d de type %d, je vais livrer\n",d->id,d->type);
        sem_post(&d->v->finCharge);
    }