#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include "Drone.h"


Vaisseau* initVaisseau(void){
    Vaisseau* v = malloc(sizeof(Vaisseau));
    sem_init(&v->garage[0], 0, 1);
    sem_init(&v->garage[1], 0, 1);
    sem_init(&v->finCharge, 0, 1);
    for(int i=0;i<NB_TYPES;i++){
        sem_init(&v->fileAttenteCharge[i], 0, 0);
    }

    v->queueGarage[0] = 0;
    v->queueGarage[1] = 0;
    v->garageOccupe = 0;
    pthread_mutex_init(&v->m[0], 0);
    pthread_mutex_init(&v->m[1], 0);
    pthread_mutex_init(&v->m[2], 0);

    nettoyer();
    for(int i = 0; i < NB_TYPES; i++){
        char c[7] = "/test";
        char d[2] = "0";
        d[0] = '0'+i;
        strcat(c, d);
        printf("%s\n", c);
        struct mq_attr* attr = NULL;
        struct mq_attr at;
        at.mq_msgsize = sizeof(Colis) + 4;
        at.mq_maxmsg = 10;
        at.mq_flags = O_RDWR;
        at.mq_curmsgs = 0;
        attr = &at;
        v->colisAttente[i] = mq_open(c, O_CREAT | O_RDWR, 0666, attr);
        printf("descripteur : %d, errno = %d\n", v->colisAttente[i], errno);
    }
    return v;
}

void nettoyer(){
    for(int i=0;i<NB_TYPES;i++){
        char c[7]="/test";
        char d[2]="0";
        d[0]='0'+i;
        strcat(c,d);
        mq_unlink(c);

    }
}

void demarrerDrones(int nb,Vaisseau* v){
//    int j, i = 0;

//    for(i = 0;i<NB_TYPES;i++){
//        for(j = 0;j < nb;j++){
//            Colis *c = malloc(sizeof(Colis));
//            *c = getColis(v);
//                creerDrone(i*(NB_TYPES-1)+j, c->type, v, c);
//        }
//    }


    node_colis* headpetite = NULL;
    node_colis* headmoyenne = NULL;
    node_colis* headgrande = NULL;
    headpetite = malloc(sizeof(node_colis));
    headmoyenne = malloc(sizeof(node_colis));
    headgrande = malloc(sizeof(node_colis));

        for(int j = 0;j < nb;j++){
            Colis *c = malloc(sizeof(Colis));
            //colis type Petite
            *c = getColis(v, PETITE);
            headpetite->col = *c;
            headpetite->next = NULL;
            //colis type Moyenne
            *c = getColis(v, MOYENNE);
            headmoyenne->col = *c;
            headmoyenne->next = NULL;
            *c = getColis(v, MOYENNE);
            add(headmoyenne, c);
            //colis type Grande
            *c = getColis(v, GRANDE);
            headgrande->col = *c;
            headgrande->next = NULL;
            *c = getColis(v, GRANDE);
            add(headgrande, c);
            *c = getColis(v, GRANDE);
            add(headgrande, c);

            creerDrone(PETITE*(NB_TYPES-1)+j, PETITE, v, headpetite);
            creerDrone(MOYENNE*(NB_TYPES-1)+j, MOYENNE, v, headmoyenne);
            creerDrone(GRANDE*(NB_TYPES-1)+j, GRANDE, v, headgrande);

    }
}

void posterColis(Vaisseau* v){

    // DRONE MOYENNE  distance client < 30 car la charge = 90
    // DRONE GRANDE  distance client < 40 car la charge = 200
    // DRONE PETITE  distance client < 20 car la charge = 40
//    node_colis* head = NULL;
//    head = malloc(sizeof(node_colis));
//    head->col = *creerColis(0, PETITE, 1, 10, 19, ENCOURS);
//    head->next = NULL;
//    add(head, creerColis(1, MOYENNE, 2, 15, 25, ENCOURS));
//    add(head, creerColis(2, GRANDE, 3, 25, 35, ENCOURS));
//    add(head, creerColis(3, MOYENNE, 4, 19, 20, ENCOURS));
//    add(head, creerColis(4, GRANDE, 5, 29, 39, ENCOURS));
//    add(head, creerColis(5, PETITE, 6, 8, 15, ENCOURS));
//
//    add(head, creerColis(6, GRANDE, 7, 30, 35, ENCOURS));
//    add(head, creerColis(7, MOYENNE, 8, 20, 29, ENCOURS));
//    add(head, creerColis(8, GRANDE, 9, 26, 38, ENCOURS));
//    add(head, creerColis(9, PETITE, 10, 9, 19, ENCOURS));
//
//    print_list(head);
//
//
//    sem_post(&v->fileAttenteCharge[GRANDE]);
//    sem_post(&v->fileAttenteCharge[MOYENNE]);
//    sem_post(&v->fileAttenteCharge[PETITE]);
//    v->c = head;

    Colis* arraycolis = malloc(sizeof(Colis) * NB_COLIS);
    arraycolis[0] = *creerColis(1, MOYENNE, 2, 15, 25, ENCOURS);
    arraycolis[1] = *creerColis(2, GRANDE, 3, 25, 35, ENCOURS);
    arraycolis[2] = *creerColis(3, MOYENNE, 4, 19, 20, ENCOURS);
    arraycolis[3] = *creerColis(4, GRANDE, 5, 29, 39, ENCOURS);
    arraycolis[4] = *creerColis(5, PETITE, 6, 8, 15, ENCOURS);
    arraycolis[5] = *creerColis(6, GRANDE, 7, 30, 35, ENCOURS);
    arraycolis[6] = *creerColis(7, MOYENNE, 8, 20, 29, ENCOURS);
    arraycolis[7] = *creerColis(8, GRANDE, 9, 26, 38, ENCOURS);
    arraycolis[8] = *creerColis(9, PETITE, 10, 9, 19, ENCOURS);

    for(int i = 0; i < NB_COLIS; i++){
        ssize_t  b = mq_send(v->colisAttente[arraycolis[i].type], (char*)&arraycolis[i], sizeof(Colis) + 4, arraycolis[i].priorite);
        printf("colis %d posté, type %d, distance %d, prio %d, message : %d, errno=%d\n", arraycolis[i].id,
               arraycolis[i].type, arraycolis[i].client, arraycolis[i].priorite, (int) b, errno);
    }


    sem_post(&v->fileAttenteCharge[GRANDE]);
    sem_post(&v->fileAttenteCharge[MOYENNE]);
    sem_post(&v->fileAttenteCharge[PETITE]);



}

