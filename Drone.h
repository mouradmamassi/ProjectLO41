#ifndef _DRONES_H_
#define _DRONES_H_
#include <semaphore.h>

#define NB_TYPES 3
#define AUTONOMIE_GRANDE 40
#define AUTONOMIE_MOYENNE 30
#define AUTONOMIE_PETITE 20
#define NB_COLIS 6
#define GRANDE 2
#define PETITE 0
#define MOYENNE 1
#define ENCOURS 0


typedef struct Colis{
    int priorite;
    int type;
    int poids;
    int client;
    int Etat;//livré | Absence Client
}Colis;

typedef struct Vaisseau{
    sem_t fileAttenteCharge[NB_TYPES],finCharge,garage[2];
    int queueGarage[2],garageOccupe;
    pthread_mutex_t m[3];
    Colis* c;
}Vaisseau;

typedef struct Drone{
    int id;
    int type;
    int charge;
    Vaisseau* v;
    Colis* c;
}Drone;


    //action colis

Colis getColis(Vaisseau* v);
Colis* creerColis(int, int, int, int, int);

    // actions drones

int creerDrone(int,int,Vaisseau*, Colis*);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);

    // actions vaisseau mère

Vaisseau* initVaisseau(void);
void demarrerDrones(int,Vaisseau*);
//void demarrerDrones(Vaisseau*);
void posterColis(Vaisseau* v);

    // gestion garage

void inscriptionGarage(Vaisseau*,int);
void entrerGarage(Vaisseau*,int);
void sortirGarage(Vaisseau*);
void appelGarage(Vaisseau*);

void erreur(const char *);

#endif