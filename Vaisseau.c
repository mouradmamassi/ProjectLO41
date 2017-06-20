
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include "Drone.h"

    Vaisseau* initVaisseau(int nbDrones,int nbColis){
        Vaisseau* v=malloc(sizeof(Vaisseau));
        v->dronesTab=malloc(sizeof(Drone*)*nbDrones);
        v->nbColisLivres=0;
        v->nbColisNonLivrables=0;
        v->nbColis=nbColis;
        v->nbDrones=nbDrones;
        sem_init(&v->garage[0],0,1);
        sem_init(&v->garage[1],0,1);
        sem_init(&v->finCharge,0,NB_QUAIS_CHARGEMENT);
        sem_init(&v->fini,0,0);
        for(int i=0;i<NB_TYPES;i++){
            sem_init(&v->fileAttenteCharge[i],0,0);
        }
        v->queueGarage[0]=0;
        v->queueGarage[1]=0;
        v->garageOccupe=0;
        pthread_mutex_init(&v->mNbColisLivres,0);
        pthread_mutex_init(&v->m[0],0);
        pthread_mutex_init(&v->m[1],0);
        pthread_mutex_init(&v->m[2],0);
    nettoyer();
        for(int i=0;i<NB_TYPES;i++){
            struct mq_attr* attr=NULL;
            struct mq_attr at;
            at.mq_msgsize=sizeof(Colis)+4;
            at.mq_maxmsg=1000;
            at.mq_flags=O_RDWR;
            at.mq_curmsgs=0;
            attr=&at;
            char c[7]="/test";
            char d[2]="0";
            d[0]='0'+i;
            strcat(c,d);
            v->colisAttente[i] = mq_open(c,O_CREAT | O_RDWR,0666,attr);
        }
        creerColis(v,nbColis);
        demarrerDrones(nbDrones,v);
        return v;
    }

    void nettoyer(void){
        for(int i=0;i<NB_TYPES;i++){
            char c[7]="/test";
            char d[2]="0";
            d[0]='0'+i;
            strcat(c,d);
            mq_unlink(c);

        }
    }

    void demarrerDrones(int nb,Vaisseau* v){
        for(int i=0;i<nb;i++){
                creerDrone(i,i%NB_TYPES,v);
        }
    }

    void posterColis(Vaisseau* v,Colis* c){
        mq_send(v->colisAttente[c->type],(char*)c,sizeof(Colis)+4,c->prio);
        printf("colis %d posté, type %d, distance %d, prio %d\n",c->no,c->type,c->distance,c->prio);
        free(c);
    }

    Colis* sortirColis(Vaisseau* v,int type){
        char b[sizeof(Colis)+4];
        mq_receive(v->colisAttente[type], b,sizeof(Colis)+4,NULL);
        Colis* c=malloc(sizeof(Colis));
        *c=*(Colis*)b;
        printf("Colis %d recu, type %d, distance %d, prio %d\n",c->no,c->type,c->distance,c->prio);
        return c;
    }

    void creerColis(Vaisseau* v, int cb){
        for(int i=0;i<cb;i++){
            Colis* c=(Colis*)malloc(sizeof(Colis));
            c->type=rand()%NB_TYPES;
            c->distance=rand()%20;
            c->no=i;
            c->prio=rand()%10;
            if(c->distance<=AUTONOMIE/2){
                posterColis(v,c);
            }
            else{
                printf("Impossible de livrer le colis %d, trop loin\n",c->no);
                free(c);
                v->nbColisNonLivrables++;
            }
        }
    }

    void finOperation(Vaisseau* v){
        printf("\n\n \t------ Fin de l'operation ------\n\n");
        printf("Livraison échouée de %d colis\n",v->nbColis-v->nbColisLivres-v->nbColisNonLivrables);
        printf("Colis non livrables : %d\n",v->nbColisNonLivrables);
        printf("Colis livrés : %d\n\n",v->nbColisLivres);
        libererTout(v);
        nettoyer();
    }

    void libererTout(Vaisseau* v){
        for(int i=0;i<v->nbDrones;i++){
            detruireDrone(v->dronesTab[i]);
        }
        free(v->dronesTab);
        free(v);
    }

    void traitant(int num){
        finOperation(gv);
        exit(0);
    }

