#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Drone.h"


int main(int argc, char *argv[]) {
    int nbDrones=1,nbColis=10;
    if(argc==3){
        nbDrones=atoi(argv[1]);
        nbColis=atoi(argv[2]);
    }
    srand(time(NULL));
    Vaisseau* v=initVaisseau();
    creerColis(v,nbColis);

    demarrerDrones(nbDrones,v);

    sleep(100);
    //nettoyer(v);
    return 0;
}