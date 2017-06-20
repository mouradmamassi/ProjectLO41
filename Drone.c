

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
        ceDrone->fret=NULL;
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
                free(d);
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
    };

    void livraison(Drone* d){
        int ou=d->fret->distance;
        aller(d,ou);
        printf("C'est le drone %d de type %d, j'attends le client\n",d->id,d->type);
        int att=rand()%10 +10;
        sleep(att);
        if(att<10){
            printf("C'est le drone %d, le client s'est pointé, colis %d livré\n",d->id,d->fret->no);
            free(d->fret);
            d->fret=NULL;
        }
        else{
            printf("C'est le drone %d, pas de client, retour du colis %d\n",d->id,d->fret->no);
        }
        aller(d,ou);
    }

    void retourVaisseau(Drone*  d){
        inscriptionGarage(d->v,1);
        entrerGarage(d->v,1);
        sortirGarage(d->v);
        if(d->fret!=NULL){
            d->fret->prio=0;
            posterColis(d->v,d->fret);
            d->fret=NULL;
        }
        printf("C'est le drone %d de type %d, je suis rentré, je recharge\n",d->id,d->type);
        sleep(AUTONOMIE-d->charge);
        d->charge=AUTONOMIE;
        printf("C'est le drone %d de type %d, je suis rechargé, j'attends\n",d->id,d->type);
    }



    void preparationLivraison(Drone* d){
        printf("C'est le drone %d de type %d, j'attends un colis\n",d->id,d->type);
        d->fret= sortirColis(d->v,d->type);
        printf("C'est le drone %d de type %d, j'ai tiré le colis %d à livre à %d\n",d->id,d->type,d->fret->no,d->fret->distance);
        sem_wait(&d->v->finCharge);
        inscriptionGarage(d->v,0);
        entrerGarage(d->v,0);
        sortirGarage(d->v);
        printf("C'est le drone %d de type %d, je charge\n",d->id,d->type);
        sleep(1);
        printf("C'est le drone %d de type %d, je vais livrer\n",d->id,d->type);
        sem_post(&d->v->finCharge);
    }
