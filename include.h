
#ifndef PROJET_ECE_CITY_INCLUDE_H
#define PROJET_ECE_CITY_INCLUDE_H



#include <stdio.h>
#include "stdlib.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#define ECRAN_WIDTH 800
#define ECRAN_HIGH 600
#define rectX1 -20
#define rectY1 60
#define LIGNE 35
#define COLONNE 45
#define BOITE_OUTIL_X 970
#define BOITE_OUTIL_Y 30
#define BOITE_OUTIL_X2 1130
#define BOITE_OUTIL_Y2 117
#define IMAGES_BOITE_OUTILS 20
#define IMAGES_ARGENT 10
#define IMAGES_POPULATION 10
#define TAILLE_CASE 20
#define WIDTH 1024
#define HIGH 678
#define OUTILS_PLATEAU_X -20
#define OUTILS_PLATEAU_Y 60
#define CASE_OUTILS_Y 260
#define TAILLE_AFFICHAGE_BOIS_X 210
#define TAILLE_AFFICHAGE_BOIS_Y 260
#define RECT_TRANSI_X 815
#define RECT_TRANSI_X2 890
#define RECT_TRANSI_Y 560
#define RECT_TRANSI_Y2 590
#define RECT_TRANSI_2_X 55
#define RECT_TRANSI_2_X2 140
#define RECT_TRANSI_2_Y 560
#define RECT_TRANSI_2_Y2 590
#define MAX_MAISON 150
#define MAX_SOMMET 300


#include "plateau.h"
#include "niveaux.h"
#include "menuDebut.h"
#include "boiteOutils.h"
#include "batiments.h"
#include "graphe.h"
#include "bfs.h"
#include "sauvegarde.h"


#endif
