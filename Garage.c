
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Drone.h"


void inscriptionGarage(Vaisseau* v,int prio){
    pthread_mutex_lock(&v->m[prio]);
    v->queueGarage[prio]++;
    pthread_mutex_unlock(&v->m[prio]);
    appelGarage(v);
    sem_wait(&v->garage[prio]);
}

void entrerGarage(Vaisseau* v,int prio){
    pthread_mutex_lock(&v->m[prio]);
    v->queueGarage[prio]--;
    pthread_mutex_unlock(&v->m[prio]);
    pthread_mutex_lock(&v->m[2]);
    v->garageOccupe=1;
    pthread_mutex_unlock(&v->m[2]);
}

void sortirGarage(Vaisseau* v){
    sleep(1);
    pthread_mutex_lock(&v->m[2]);
    v->garageOccupe=0;
    pthread_mutex_unlock(&v->m[2]);
    appelGarage(v);
}

void appelGarage(Vaisseau* v){
    pthread_mutex_lock(&v->m[2]);
    if(!v->garageOccupe){
        pthread_mutex_unlock(&v->m[2]);
        pthread_mutex_lock(&v->m[1]);
        if(v->queueGarage[1]>0){
            pthread_mutex_unlock(&v->m[1]);
            sem_post(&v->garage[1]);
        }
        else{
            pthread_mutex_lock(&v->m[0]);
            if(v->queueGarage[0]>0){
                pthread_mutex_unlock(&v->m[0]);
                sem_post(&v->garage[0]);
            }
            pthread_mutex_unlock(&v->m[0]);
        }
        pthread_mutex_unlock(&v->m[1]);
    }
    pthread_mutex_unlock(&v->m[2]);
}