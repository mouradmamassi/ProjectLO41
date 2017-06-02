

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "Drone.h"


    int creerDrone(int id, int type){
        pthread_t th;
        Drone *ceDrone=malloc(sizeof(Drone));
        ceDrone->id=id;
        ceDrone->charge=100;
        ceDrone->type=type;
        if (pthread_create(&th, 0, actionDrone, (void *) ceDrone) != 0)
            erreur("Erreur Creation thread");
        return (int)th;

    }

    void erreur(const char *msg)
    {
        perror(msg);
        exit(1);
    }

    void* actionDrone(void* data){
        Drone* d=(Drone*)data;
        printf("C'est le drone %d de type %d, j'attends\n",d->id,d->type);
        sem_wait(&fileAttenteCharge[d->type]);
        printf("C'est le drone %d de type %d, j'attends pour charger\n",d->id,d->type);
        sem_wait(&finCharge);
        printf("C'est le drone %d de type %d, je charge\n",d->id,d->type);
        sleep(1);
        printf("C'est le drone %d de type %d, je libère la place\n",d->id,d->type);
        sem_post(&finCharge);
        pthread_exit(0);
    };