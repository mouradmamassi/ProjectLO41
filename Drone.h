
#include <semaphore.h>

#define NB_TYPES 3
#define AUTONOMIE 28

//sem_t fileAttenteCharge[NB_TYPES],finCharge,garage;

typedef struct Drone{
    int id;
    int type;
    int charge;
    struct Vaisseau* v;
}Drone;

typedef struct Vaisseau{
    sem_t fileAttenteCharge[NB_TYPES],finCharge,garage;
}Vaisseau;

int creerDrone(int,int,Vaisseau*);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);
void erreur(const char *);

Vaisseau* initVaisseau(void);
void demarrerDrones(int,Vaisseau*);
void posterColis(Vaisseau* v);