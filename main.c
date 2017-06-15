#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Drone.h"


int main() {
    srand(time(NULL));
    Vaisseau* v=initVaisseau();
    creerColis(v,10);

    demarrerDrones(2,v);

    sleep(100);
    //nettoyer(v);
    return 0;
}