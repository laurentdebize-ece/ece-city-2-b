//
// Created by Louis Duvanel on 15/11/2022.
//

#ifndef PROJET_ECE_CITY_GRAPHE_H
#define PROJET_ECE_CITY_GRAPHE_H




Graphe* creerGraphe (Principal* p);
void ajoutSommet (Principal* p, int i, int j);
void detectionContours (Principal* p, pSommet s);
void creerArete(Principal* p, int s1, int numeroSommetDeProximite);
void affichageGraphe(Principal* p);
void lierArete(Arete* arete, struct Sommet* sPrev);
void ajouterTableauCase(Principal* p, int i, int j, int numSommet);
void verifierNumeroProximite(Principal* p, int j, int i);
#endif //PROJET_ECE_CITY_GRAPHE_H
