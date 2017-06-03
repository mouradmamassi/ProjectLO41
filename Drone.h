
#include <semaphore.h>

#define NB_TYPES 3

pthread_mutex_t m;
sem_t fileAttenteCharge[NB_TYPES],finCharge,garage;

typedef struct Drone{
    int id;
    int type;
    int charge;
}Drone;

int creerDrone(int,int);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);
void erreur(const char *);

void demarrerDrones(int);
void posterColis(void);