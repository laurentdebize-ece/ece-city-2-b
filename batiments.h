//
// Created by Louis Duvanel on 07/11/2022.
//

#ifndef PROJET_ECE_CITY_BATIMENTS_H
#define PROJET_ECE_CITY_BATIMENTS_H

void initialisationLongueursLargeur(Principal* p);
void dessinFantomeMaison(Principal* p, int i, int j, ALLEGRO_COLOR couleurBat);
void dessinFantomeAutreConstruction(Principal* p, ALLEGRO_COLOR couleurBat, int i, int j);
void dessinFantomeParc(Principal* p, int i, int j);

#endif //PROJET_ECE_CITY_BATIMENTS_H
