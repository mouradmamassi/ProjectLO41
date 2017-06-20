#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include "Drone.h"

int main(int argc, char *argv[]) {
    int nbDrones=1,nbColis=10;
    if(argc==3){
        nbDrones=atoi(argv[1]);
        printf("%d\n",nbDrones);
        nbColis=atoi(argv[2]);
    }
    srand(time(NULL));
    Vaisseau* v=initVaisseau(nbDrones,nbColis);
    gv=v;
    signal(SIGINT,traitant);
    sem_wait(&v->fini);
    finOperation(v);
    return 0;
}