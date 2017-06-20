
#include <semaphore.h>
#include <mqueue.h>

#define NB_TYPES 3
#define AUTONOMIE 30
#define NB_QUAIS_CHARGEMENT 3

typedef struct Colis{
    int no;
    int type;
    int distance;
    unsigned int prio;
}Colis;

typedef struct Drone{
    int id;
    int type;
    int charge;
    struct Vaisseau* v;
    Colis* fret;
}Drone;

typedef struct Vaisseau{
    sem_t fileAttenteCharge[NB_TYPES],finCharge,garage[2],fini;
    int queueGarage[2],garageOccupe;
    pthread_mutex_t m[3],mNbColisLivres;
    mqd_t colisAttente[NB_TYPES];
    int nbColisLivres, nbColisNonLivrables,nbColis,nbDrones;
    Drone** dronesTab;
}Vaisseau;

Vaisseau* gv;

    // actions drones

int creerDrone(int,int,Vaisseau*);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);
void detruireDrone(Drone* d);

    // actions vaisseau mère

Vaisseau* initVaisseau(int nbDrones, int nbColis);
void demarrerDrones(int,Vaisseau*);
void posterColis(Vaisseau*,Colis*);
void creerColis(Vaisseau*, int);
Colis* sortirColis(Vaisseau*,int);
void nettoyer(void);
void finOperation(Vaisseau* v);
void libererTout(Vaisseau* v);

    // gestion garage

void inscriptionGarage(Vaisseau*,int);
void entrerGarage(Vaisseau*,int);
void sortirGarage(Vaisseau*);
void appelGarage(Vaisseau*);

void erreur(const char *);
void traitant(int num);