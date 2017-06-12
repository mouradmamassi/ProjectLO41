#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Drone.h"


int main() {
    srand(time(NULL));
    Vaisseau* v = initVaisseau();
    posterColis(v);
    demarrerDrones(1, v);



//    for(int i=0;i<10;i++){

//    }


    sleep(100);
    return 0;
}