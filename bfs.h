#ifndef PROJET_ECE_CITY_BFS_H
#define PROJET_ECE_CITY_BFS_H

#include "plateau.h"

typedef struct maillon{
    int numSommet;
    struct maillon* next;
} Maillon;


typedef struct File{
    Maillon* first;
    Maillon* last;
}File;

void bfs(Principal *p, int num);
void affichageFile(File* file);
void bfsElectricite(Principal *p, int num);
void remiseAZero(Principal* p);
#endif