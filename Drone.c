

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
        inscriptionGarage(d->v,1);
        entrerGarage(d->v,1);
        sortirGarage(d->v);
        printf("C'est le drone %d de type %d, je suis rentré, je recharge\n",d->id,d->type);
        sleep(AUTONOMIE-d->charge);
        d->charge=AUTONOMIE;
        printf("C'est le drone %d de type %d, je suis rechargé, j'attends\n",d->id,d->type);
    }

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

    void preparationLivraison(Drone* d){
        sem_wait(&d->v->fileAttenteCharge[d->type]);
        printf("C'est le drone %d de type %d, j'attends pour charger\n",d->id,d->type);
        sem_wait(&d->v->finCharge);
        inscriptionGarage(d->v,0);
        entrerGarage(d->v,0);
        sortirGarage(d->v);
        printf("C'est le drone %d de type %d, je charge\n",d->id,d->type);
        sleep(1);
        printf("C'est le drone %d de type %d, je vais livrer\n",d->id,d->type);
        sem_post(&d->v->finCharge);
    }
