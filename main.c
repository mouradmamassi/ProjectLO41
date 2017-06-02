#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Drone.h"


int main() {
    srand(time(NULL));
    demarrerDrones(2);
    sleep(1);
    sem_post(&finCharge);
    for(int i=0;i<10;i++){
        posterColis();
    }

    sleep(100);
    return 0;
}