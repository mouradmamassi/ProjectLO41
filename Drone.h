
#include <semaphore.h>

#define NB_TYPES 3
#define AUTONOMIE 28
#define NB_Colis 2
#define NB_Slot 2

typedef struct Colis{
    int priorite;
    int poids;
    int client;
    int Etat;//livré | Absance Client
    int nslot;//slot utiliser pour livre un colis
}Colis;

typedef struct Drone{
    int id;
    int type;
    int charge;
    struct Vaisseau* v;
    Colis c;
}Drone;
typedef struct Slot{
    int aller;
    int retour;
}Slot;

typedef struct Vaisseau{
    sem_t fileAttenteCharge[NB_TYPES],finCharge,garage[2];
    int queueGarage[2],garageOccupe;
    pthread_mutex_t m[3];
    Colis* c;
    Slot * s;//TODO add Slot au initVaisseau

}Vaisseau;

int creerDrone(int,int,Vaisseau*);
Colis getColis(Vaisseau* v);
Colis creerColis(int, int, int, int, int);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);
void erreur(const char *);

Vaisseau* initVaisseau(void);
void demarrerDrones(int,Vaisseau*);
void posterColis(Vaisseau* v);

void inscriptionGarage(Vaisseau*,int);
void entrerGarage(Vaisseau*,int);
void sortirGarage(Vaisseau*);
void appelGarage(Vaisseau*);