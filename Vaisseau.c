
#include <stdlib.h>
#include <stdio.h>

#include "Drone.h"

    void demarrerDrones(int nb){
        for(int i=0;i<NB_TYPES;i++){
            for(int j=0;j<nb;j++){
                creerDrone(i*(NB_TYPES-1)+j,i);
            }
        }
    }

    int creerColis(void){
        return rand() % 3;
    }

    void posterColis(void){
        int colis=creerColis();
        printf("J'ai un colis de type %d\n",colis);
        sem_post(&fileAttenteCharge[colis]);
    }