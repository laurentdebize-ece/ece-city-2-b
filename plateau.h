//
// Created by Louis Duvanel on 01/11/2022.
//

#ifndef PROJET_ECE_CITY_PLATEAU_H
#define PROJET_ECE_CITY_PLATEAU_H

#include "include.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>


/// TYPE DE BATIMENTS///
/// 1 : ROUTE
/// 2 : MAISON
/// 3 : CENTRALE ELEC
/// 4 : CHATEAU D'EAU
/// 5 : CASERNE DE POMPIER

typedef struct Coordonnees {
    int x, y;
} Coordonnees;

typedef struct Batiments{
    int largeurY;
    int longueurX;
    int typeBatiments;
    Coordonnees contour;
    int stadeDeConstruction;
    Coordonnees tab[12] ;
    int capaciteElec ;
    int capaciteEau;
    int numeroCentraleAssociee;
    int numeroChateauAssociee;
} Batiments;

typedef struct Case {
    int positionX;
    int positionY;
    int* adjacence;
    Batiments batiment;
    int occupee;
    int numeroSommetProximite;
    ALLEGRO_COLOR caseCouleur;
    int canalisation, route, construction; // ces param. seront utilisés comme des Booléens
    // cad ils seront à  1 (vrai) ou -1 (faux)
} Case;

typedef struct Parametre{
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* police;
    ALLEGRO_EVENT event;
}Parametre;

/// Structure dans plateau.h puisqu'on souhaite avoir le graphe dans la structure Principale
typedef struct Arete {
    int numSommetNext;
    struct Arete* areteSuivante;
}Arete;

typedef struct Arete* pArete;


struct Sommet{
    int colonne, ligne;
    Batiments construction;
    char couleur;
    Arete* arete;
    int numero;
    int nombreHabitant;
    int stadeConstruction;
    bool accesElecValide;
    int habitantSansEau;// > 0 si des habitants de l'habitation ont pas d'eau
    int habitantSansElec;
    bool accesEauValide; // == true si TOUS les habitants ont de l'eau && == false si habitantSansEau > 0
}Sommet;

typedef struct Sommet* pSommet;

typedef struct Graphe {
    int taille;
    int ordre;
    pSommet* pSommet;
} Graphe;





typedef struct Principal {
    int modeDeJeu ; // == 0 si Communiste ==1 si capitaliste
    bool leJeuEstLance;
    int occupe ;
    Case plateau[LIGNE][COLONNE];
    int choix;
    int argentbanque;
    int nbrhabitants;

    ALLEGRO_BITMAP* affichageBoiteOutils [IMAGES_BOITE_OUTILS];
    ALLEGRO_BITMAP* barreArgent[IMAGES_ARGENT];
    ALLEGRO_BITMAP* barrePopulation[IMAGES_POPULATION];
    ALLEGRO_BITMAP* tabfondjeu[6];
    ALLEGRO_BITMAP* immeuble ;
    ALLEGRO_BITMAP* house ;
    ALLEGRO_BITMAP* gratteciel ;
    ALLEGRO_BITMAP* terrain_vague ;
    ALLEGRO_BITMAP* cabane ;
    ALLEGRO_BITMAP* caserne ;
    ALLEGRO_BITMAP* parc;
    ALLEGRO_BITMAP* ruine;
    ALLEGRO_BITMAP* chateau ;
    ALLEGRO_BITMAP* school;
    ALLEGRO_BITMAP* library;
    ALLEGRO_BITMAP* bouton_quitter ;
    ALLEGRO_BITMAP* bouton_quitter2 ;
    ALLEGRO_BITMAP* quitter ;
    ALLEGRO_FONT* affichageTexteBoiteOutils;
    ALLEGRO_TIMER* timer ;
    ALLEGRO_FONT* police2 ;
    FILE* informations ;
    FILE* fichiersauvegarde ;
    int sectimer ;
    int mintimer ;
    FILE* ftexte ;
    int capElec , capEau ;
    ALLEGRO_BITMAP* imageBoiteOutils;
    ALLEGRO_BITMAP* centrale ;
    Parametre parametre;
    bool secondePageOutils;
    bool troisiemePageOutils;
    bool niveau1 ;
    bool enCoursdeConstruction;
    bool niveau2 ;
    bool sourisBoutonQuitter;
    bool quitterpartie ;
    bool sauvegarder ;
    bool erreur;
    bool erreur2;
    /// booléen utilisé pour charger une nouvelle partie ou celle sauvegarder
    bool reprendrepartie ;
    bool anciennepartiecharge;
    Graphe* g;
    int* maisonProche;
    int compteurMaison;//Compteur pour le tableau de maison proche
    int* chateauEau;
    int* centraleElectrique;
    int compteurChateau;
    int compteurCentrale;
} Principal;


bool detectionViabilite(Principal * p , int i , int j , int k);

void initialisationPlateau(Principal *p);
void dessinerMatrice(Principal* p);
void initialisationstructure(Principal *p);
void affichageMatrice(Principal* p);
void comptetimer(Principal* p , ALLEGRO_EVENT event) ;
void placementconstruction(Principal* p , int x , int y) ;
void affichageInteractifSouris(Principal* p, int x, int y);
void agrandissementBatiment(Principal* p);
void affichageInformation(Principal* p, ALLEGRO_FONT* police);
void placementIndiceDeProximite(Principal* p, int i, int j);
void initialisationImagesArgent(Principal* p);
int barreInteractifArgent(Principal* p);
void initialisationImagesPopulation(Principal* p);
int barreInteractifPopulation(Principal* p);
void liberer(Principal * p);
void initialisationPrincipale(Principal *p);
void impots(Principal* p);
void miseAJourDuNombreHabitantParSommet(Principal* p);


#endif //PROJET_ECE_CITY_PLATEAU_H
