#ifndef _DRONES_H_
#define _DRONES_H_
#include <semaphore.h>
#include <mqueue.h>

#define NB_TYPES 3
#define AUTONOMIE_GRANDE 200
#define AUTONOMIE_MOYENNE 90
#define AUTONOMIE_PETITE 40
#define GRANDE 2
#define PETITE 0
#define MOYENNE 1
#define NOLIVRE -1


typedef struct Colis{
    int id;
    int priorite;
    int type;
    int poids;
    int client;
    int Etat;//livré | Absence Client
}Colis;

typedef struct node{
    Colis col;
    struct node * next;
}node_colis;

typedef struct Drone{
    int id;
    int type;
    int charge;
    struct Vaisseau* v;
    node_colis* clist;
}Drone;

typedef struct Vaisseau{
    sem_t fileAttenteCharge[NB_TYPES], finCharge, garage[2], fini;
    int queueGarage[2], garageOccupe;
    pthread_mutex_t m[3], mNbColisLivres;
    mqd_t colisAttente[NB_TYPES];
    int nbColisLivres, nbColisNonLivrables, nbColis, nbDrones;
    Drone** dronesTab;
}Vaisseau;

Vaisseau* gv;


//list
void print_list(node_colis*);
void add(node_colis*, Colis*);
int remove_by_position(node_colis**, int);
//action colis

Colis getColis(Vaisseau* v, int);
Colis* creerColis(int, int);
void CreerAndPosterColis(Vaisseau* v, int nbColis);

// actions drones

int creerDrone(int,int, Vaisseau*);
void* actionDrone(void*);
void preparationLivraison(Drone*);
void retourVaisseau(Drone*);
void livraison(Drone*);
void detruireDrone(Drone* d);

// actions vaisseau mère

Vaisseau* initVaisseau(int nbDrones, int nbColis);
void demarrerDrones(int,Vaisseau*);
void posterColis(Vaisseau* v,int c);
void nettoyer();
void finOperation(Vaisseau* v);
void libererTout(Vaisseau* v);

// gestion garage

void inscriptionGarage(Vaisseau*,int);
void entrerGarage(Vaisseau*,int);
void sortirGarage(Vaisseau*);
void appelGarage(Vaisseau*);
void erreur(const char *);
void traitant();



#endif