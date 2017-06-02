
#include <semaphore.h>

#define NB_TYPES 3

pthread_mutex_t m;
sem_t fileAttenteCharge[NB_TYPES],finCharge;

typedef struct Drone{
    int id;
    int type;
    int charge;
}Drone;

int creerDrone(int,int);
void* actionDrone(void*);
void erreur(const char *);

void demarrerDrones(int);
void posterColis(void);