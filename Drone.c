#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "Drone.h"

Colis* creerColis(int id, int typ, int p, int poid, int cl, int e){
    Colis* c = malloc(sizeof(Colis));
    c->id = id;
    c->type = typ;
    c->priorite = p;
    c->poids = poid;
    c->client = cl;
    c->Etat = e;

    return c;
}

Colis getColis(Vaisseau* v, int type){

    node_colis* c = v->c;
    Colis colis;
    int pre = NB_COLIS;
    while(c != NULL){
        if(c->col.Etat == ENCOURS && c->col.type == type) {
            if (c->col.priorite <= pre) {
                colis = c->col;
                pre = c->col.priorite;
            }
        }
        c = c->next;
    }
    printf("colis id %d  est bien charger \n", remove_by_position(&v->c, colis.id));

    return colis;

}
int creerDrone(int id, int type, Vaisseau* v, node_colis* c){
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

    node_colis* current = d->c;
    int destination = 0;
    aller(d, current->col.client);
    while(current != NULL){
        d->charge -= destination;

        printf("drone %d  deplacer la charge est %d \n", d->id, d->charge);

        printf("C'est le drone %d de type %d, je suis chez le client\n", d->id, d->type);
        int etat_colis = rand()%(2) + 1;

        if(etat_colis == 1)
            etat_colis = rand()%(2) + 1;

        if(etat_colis == 2) {
            printf("C'est le drone %d de type %d, je suis chez le client, Colis %d bien livré  \n", d->id, d->type,
                   current->col.id);
            remove_by_position(&d->c, current->col.id);
        }

        if(etat_colis == 1) {
            printf("C'est le drone %d de type %d, je suis chez le client, Colis %d n'est pas livré \n", d->id, d->type,
                   current->col.id);
            current->col.Etat = NOLIVRE;
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

void VaisseauEtatColis(Drone * d){
    node_colis* current = d->c;
    while(current != NULL){
        if(current->col.Etat == NOLIVRE)
            add(d->v->c, current->col.id);

        current = current->next;
    }

    print_list(d->v);

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
