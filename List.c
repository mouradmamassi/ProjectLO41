#include <stdio.h>
#include <stdlib.h>
#include "Drone.h"
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