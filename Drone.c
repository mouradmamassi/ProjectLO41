#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "Drone.h"

void detruireDrone(Drone* d){
    if(d->clist != NULL){
        free(d->clist);
    }
    if(d != NULL){
        free(d);
    }

}

Colis getColis(Vaisseau* v, int type){

    char b[sizeof(Colis) + 4];
    Colis* c = malloc(sizeof(Colis));
    if(mq_receive(v->colisAttente[type], b, sizeof(Colis) + 4, NULL) == -1){
        c->id = -1;
        return *c;
    }
    *c = *(Colis*) b;
    printf("Colis %d recu, type %d, distance %d, prio %d \n", c->id, c->type, c->client, c->priorite);
    return *c;

}
int creerDrone(int id, int type,Vaisseau* v){
    pthread_t th;
    node_colis* listcolis = malloc(sizeof(node_colis));
    Colis* c = malloc(sizeof(Colis));
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
    for(int i = 0; i < 3; i++){
        *c = getColis(v, type);
        if(c->id != -1)
            add(listcolis, c);
    }
    ceDrone->clist = listcolis;
    v->dronesTab[id] = ceDrone;
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

    node_colis* current = d->clist;
    int destination = 0;
    aller(d, current->col.client);
    while(current != NULL){
        d->charge -= destination;

        printf("drone %d  deplacer la charge est %d \n", d->id, d->charge);

        printf("C'est le drone %d de type %d, j'attends le client\n",d->id,d->type);
        int att=rand()%10+1;
        sleep(att);

        if(att<10){
            printf("C'est le drone %d de type %d, je suis chez le client, Colis %d bien livré  \n", d->id, d->type,
                   current->col.id);
            remove_by_position(&d->clist, current->col.id);

            pthread_mutex_lock(&d->v->mNbColisLivres);
            d->v->nbColisLivres++;
            pthread_mutex_unlock(&d->v->mNbColisLivres);
            if( (d->v->nbColisLivres + d->v->nbColisNonLivrables) == d->v->nbColis){
                sem_post(&d->v->fini);
            }
        }else{

            current->col.Etat = NOLIVRE;
            printf("C'est le drone %d, pas de client, retour du colis %d\n",d->id,current->col.id);
        }

        destination = current->col.client;
        current = current->next;

    }

    aller(d, destination); //back home




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
