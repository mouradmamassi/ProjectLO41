
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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
    return v;
}

void demarrerDrones(int nb,Vaisseau* v){
    int j, i = 0;

    for(i = 0;i<NB_TYPES;i++){
        for(j = 0;j < nb;j++){
            Colis *c = malloc(sizeof(Colis));
            *c = getColis(v);
                creerDrone(i*(NB_TYPES-1)+j, c->type, v, c);
        }
    }
}

void posterColis(Vaisseau* v){

    node_colis* head = NULL;
    head = malloc(sizeof(node_colis));
    head->col = *creerColis(0, PETITE, 1, 20, 3, ENCOURS);
    head->next = NULL;
    add(head, creerColis(1, MOYENNE, 2, 2, 4, ENCOURS));
    add(head, creerColis(2, GRANDE, 3, 2, 3, ENCOURS));
    add(head, creerColis(3, MOYENNE, 4, 2, 4, ENCOURS));
    add(head, creerColis(4, GRANDE, 5, 2, 3, ENCOURS));
    add(head, creerColis(5, PETITE, 6, 2, 4, ENCOURS));

    print_list(head);
    node_colis* current = head;
    while(current != NULL){
        printf("J'ai un colis de type %d\n",current->col.type);
        sem_post(&v->fileAttenteCharge[current->col.type]);
        current = current->next;
    }
    v->c = head;
}

