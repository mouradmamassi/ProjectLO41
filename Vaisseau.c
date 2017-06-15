
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include "Drone.h"

    Vaisseau* initVaisseau(void){
        Vaisseau* v=malloc(sizeof(Vaisseau));
        sem_init(&v->garage[0],0,1);
        sem_init(&v->garage[1],0,1);
        sem_init(&v->finCharge,0,1);
        for(int i=0;i<NB_TYPES;i++){
            sem_init(&v->fileAttenteCharge[i],0,0);
        }
        v->queueGarage[0]=0;
        v->queueGarage[1]=0;
        v->garageOccupe=0;
        pthread_mutex_init(&v->m[0],0);
        pthread_mutex_init(&v->m[1],0);
        pthread_mutex_init(&v->m[2],0);
    nettoyer(v);
        for(int i=0;i<NB_TYPES;i++){
            char c[7]="/test";
            char d[2]="0";
            d[0]='0'+i;
            strcat(c,d);
            printf("%s\n",c);
            struct mq_attr* attr=NULL;
            struct mq_attr at;
            at.mq_msgsize=sizeof(Colis)+4;
            at.mq_maxmsg=10;
            at.mq_flags=O_RDWR;
            at.mq_curmsgs=0;
            attr=&at;
            v->colisAttente[i] = mq_open(c,O_CREAT | O_RDWR,0666,attr);
            printf("descripteur : %d, errno=%d\n",v->colisAttente[i],errno);
        }


        return v;

    }

    void nettoyer(Vaisseau* v){
        for(int i=0;i<NB_TYPES;i++){
            char c[7]="/test";
            char d[2]="0";
            d[0]='0'+i;
            strcat(c,d);
            mq_unlink(c);

        }
    }

    void demarrerDrones(int nb,Vaisseau* v){
        for(int i=0;i<NB_TYPES;i++){
            for(int j=0;j<nb;j++){
                creerDrone(i*(NB_TYPES-1)+j,i,v);
            }
        }
    }

    void posterColis(Vaisseau* v,Colis* c){
        ssize_t  b = mq_send(v->colisAttente[c->type],(char*)c,sizeof(Colis)+4,c->prio);
        printf("colis %d posté, type %d, distance %d, prio %d, message : %d, errno=%d\n",c->no,c->type,c->distance,c->prio,(int) b,errno);
        free(c);
    }

    Colis* sortirColis(Vaisseau* v,int type){
        char b[sizeof(Colis)+4];
        int a =mq_receive(v->colisAttente[type], b,sizeof(Colis)+4,NULL);
        Colis* c=malloc(sizeof(Colis));
        *c=*(Colis*)b;
        printf("Colis %d recu, type %d, distance %d, prio %d, message %d, errno%d\n",c->no,c->type,c->distance,c->prio,a,errno);
        return c;
    }

    void creerColis(Vaisseau* v, int cb){
        for(int i=0;i<cb;i++){
            Colis* c=(Colis*)malloc(sizeof(Colis));
            c->type=rand()%NB_TYPES;
            c->distance=rand()%20;
            c->no=i;
            c->prio=rand()%10;
            posterColis(v,c);
        }
    }

    void appelerDrone(Vaisseau *v){
        int colis=1;
        printf("J'ai un colis de type %d\n",colis);
        sem_post(&v->fileAttenteCharge[colis]);
    }
