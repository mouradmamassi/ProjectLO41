
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

void print_list(node_colis* head){
    node_colis* current = head;
    while(current != NULL){
        printf("Id colis est %d Etat est %d \n", current->col.id, current->col.Etat);
        current = current->next;
    }
}

void add(node_colis * head, Colis* col) {
    node_colis * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_colis));
    current->next->col = *col;
    current->next->next = NULL;
}

int remove_by_position(node_colis** head, int id){

    int retourval = -1;
    node_colis* current = (*head)->next;
    node_colis* previous = *head;
    node_colis* temp = NULL;

    if ( ((*head)->col.id) == id) {
        temp = *head;
        retourval = temp->col.id;
        *head = (*head)->next;
        free(temp);
        return retourval;
    }

    while (current != NULL && previous != NULL) {
        printf("\n %d IDDD \n ", current->col.id);
        if(current->col.id == id){
            temp = current;
            retourval = temp->col.id;
            previous->next = current->next;
            free(temp);
            return retourval;
        }
        previous = current;
        current = current->next;

    }


    return retourval;

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

