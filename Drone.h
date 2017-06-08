
#include <semaphore.h>

#define NB_TYPES 3
#define AUTONOMIE 28

typedef struct Drone{
    int id;
    int type;
    int charge;
    struct Vaisseau* v;
}Drone;

typedef struct Vaisseau{
    sem_t fileAttenteCharge[NB_TYPES],finCharge,garage[2];
    int queueGarage[2],garageOccupe;
    pthread_mutex_t m[3];
}Vaisseau;

    // actions drones

int creerDrone(int,int,Vaisseau*);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);

    // actions vaisseau mère

Vaisseau* initVaisseau(void);
void demarrerDrones(int,Vaisseau*);
void posterColis(Vaisseau* v);

    // gestion garage

void inscriptionGarage(Vaisseau*,int);
void entrerGarage(Vaisseau*,int);
void sortirGarage(Vaisseau*);
void appelGarage(Vaisseau*);

void erreur(const char *);