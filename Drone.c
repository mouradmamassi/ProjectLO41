#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "Drone.h"

    Colis* creerColis(int typ, int p, int po, int cl, int e){
        Colis* c = malloc(sizeof(Colis));
        c->type = typ;
        c->priorite = p;
        c->poids = po;
        c->client = cl;
        c->Etat = e;

        return c;
    }

    Colis getColis(Vaisseau* v){

        Colis * c = v->c;
        Colis colis;
        int pre = NB_COLIS, i, position;
        for( i = 0; i < NB_COLIS; i++){
            if(c[i].Etat == ENCOURS) {
                if(c[i].priorite != 0 && c[i].priorite <= pre){
                    pre = c[i].priorite;
                    position = i;
                }
            }
        }
        colis = c[position];
        c[position].priorite = 0;
        return colis;

    }
    int creerDrone(int id, int type, Vaisseau* v, Colis* c){
        pthread_t th;
        Drone *ceDrone = malloc(sizeof(Drone));
        ceDrone->id = id;
        if(type == GRANDE)
            ceDrone->charge = AUTONOMIE_GRANDE;
        else if(type == MOYENNE)
            ceDrone->charge = AUTONOMIE_MOYENNE;
        else if(type == PETITE)
            ceDrone->charge = AUTONOMIE_PETITE;
        ceDrone->type = type;
        ceDrone->v = v;
        ceDrone->c = c;
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
        aller(d,d->c->client);
        printf("C'est le drone %d de type %d, je suis chez le client\n",d->id,d->type);
        d->c->Etat = rand()%(2) + 1;
        if(d->c->Etat == 2)
            printf("C'est le drone %d de type %d, je suis chez le client, Colis bien livré  \n",d->id,d->type);
        if(d->c->Etat == 1)
            printf("C'est le drone %d de type %d, je suis chez le client, Colis n'est pas livré \n",d->id,d->type);

        aller(d,d->c->client);
    }

    void chargerDrone(Drone* d){
        if(d->type == MOYENNE){
            sleep(AUTONOMIE_MOYENNE-d->charge);
            d->charge = AUTONOMIE_MOYENNE;
        }
        else if(d->type == PETITE){
            sleep(AUTONOMIE_PETITE-d->charge);
            d->charge = AUTONOMIE_PETITE;
        }
        else if(d->type == GRANDE){
            sleep(AUTONOMIE_GRANDE-d->charge);
            d->charge = AUTONOMIE_GRANDE;
        }
    }
    void retourVaisseau(Drone*  d){
        inscriptionGarage(d->v,1);
        entrerGarage(d->v,1);
        sortirGarage(d->v);
        printf("C'est le drone %d de type %d, je suis rentré, je recharge\n",d->id,d->type);
        chargerDrone(d);
        printf("C'est le drone %d de type %d, je suis rechargé, j'attends\n",d->id,d->type);
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
