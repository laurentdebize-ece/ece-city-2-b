//
// Created by Louis Duvanel on 07/11/2022.
//

#include "include.h"

void initialisationLongueursLargeur(Principal* p){
    for(int i =0;i<LIGNE;i++){
        for(int j=0;j<COLONNE;j++){
            switch(p->plateau[i][j].batiment.typeBatiments) {
                case 0 :
                    p->plateau[i][j].batiment.longueurX = TAILLE_CASE;
                    p->plateau[i][j].batiment.largeurY = TAILLE_CASE;
                    break;
                case 1 :
                    p->plateau[i][j].batiment.longueurX = 3 * TAILLE_CASE;
                    p->plateau[i][j].batiment.largeurY = 3 * TAILLE_CASE;
                    break;
                default :
                    p->plateau[i][j].batiment.longueurX = 6 * TAILLE_CASE;
                    p->plateau[i][j].batiment.largeurY = 4 * TAILLE_CASE;
            }
        }
    }
}

//Dessin de la maison autour d'un point

void dessinFantomeMaison(Principal* p, int i, int j, ALLEGRO_COLOR couleurBat) {
    if (i != 0 && i != 34 && j != 0 && j != 44) {
        al_draw_filled_rectangle(p->plateau[i][j].positionX, p->plateau[i][j].positionY,
                                 p->plateau[i][j].positionX + TAILLE_CASE, p->plateau[i][j].positionY + TAILLE_CASE,
                                 couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j].positionX, p->plateau[i - 1][j].positionY,
                                 p->plateau[i - 1][j].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j].positionX, p->plateau[i + 1][j].positionY,
                                 p->plateau[i + 1][j].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j - 1].positionX, p->plateau[i - 1][j - 1].positionY,
                                 p->plateau[i - 1][j - 1].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j - 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i][j - 1].positionX, p->plateau[i][j - 1].positionY,
                                 p->plateau[i][j].positionX + TAILLE_CASE, p->plateau[i][j - 1].positionY + TAILLE_CASE,
                                 couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j + 1].positionX, p->plateau[i + 1][j + 1].positionY,
                                 p->plateau[i + 1][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j + 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j - 1].positionX, p->plateau[i + 1][j - 1].positionY,
                                 p->plateau[i + 1][j - 1].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j - 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i][j + 1].positionX, p->plateau[i][j + 1].positionY,
                                 p->plateau[i][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i][j + 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j + 1].positionX, p->plateau[i - 1][j + 1].positionY,
                                 p->plateau[i - 1][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j + 1].positionY + TAILLE_CASE, couleurBat);
    }
}

void dessinFantomeParc(Principal* p, int i, int j){
    al_draw_filled_rectangle(p->plateau[i][j].positionX, p->plateau[i][j].positionY, p->plateau[i][j].positionX+TAILLE_CASE, p->plateau[i][j].positionY+TAILLE_CASE,
                             al_map_rgba(176, 242, 182, 100));
    al_draw_filled_rectangle(p->plateau[i-1][j].positionX, p->plateau[i-1][j].positionY, p->plateau[i-1][j].positionX+TAILLE_CASE, p->plateau[i-1][j].positionY+TAILLE_CASE,al_map_rgba(176, 242, 182, 100));
    al_draw_filled_rectangle(p->plateau[i-1][j+1].positionX, p->plateau[i-1][j+1].positionY, p->plateau[i-1][j+1].positionX+TAILLE_CASE, p->plateau[i-1][j+1].positionY+TAILLE_CASE,al_map_rgba(176, 242, 182, 100));
    al_draw_filled_rectangle(p->plateau[i][j+1].positionX, p->plateau[i][j+1].positionY, p->plateau[i][j+1].positionX+TAILLE_CASE, p->plateau[i][j+1].positionY+TAILLE_CASE,al_map_rgba(176, 242, 182, 100));
}

void dessinFantomeAutreConstruction(Principal* p, ALLEGRO_COLOR couleurBat, int i, int j){
    if ((i > 1 && j > 2 && i < 34 && j < 43)) {
        al_draw_filled_rectangle(p->plateau[i][j].positionX, p->plateau[i][j].positionY,
                                 p->plateau[i][j].positionX + TAILLE_CASE, p->plateau[i][j].positionY + TAILLE_CASE,
                                 couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j].positionX, p->plateau[i - 1][j].positionY,
                                 p->plateau[i - 1][j].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j].positionX, p->plateau[i + 1][j].positionY,
                                 p->plateau[i + 1][j].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j - 1].positionX, p->plateau[i - 1][j - 1].positionY,
                                 p->plateau[i - 1][j - 1].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j - 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i][j - 1].positionX, p->plateau[i][j - 1].positionY,
                                 p->plateau[i][j].positionX + TAILLE_CASE, p->plateau[i][j - 1].positionY + TAILLE_CASE,
                                 couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j + 1].positionX, p->plateau[i + 1][j + 1].positionY,
                                 p->plateau[i + 1][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j + 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j - 1].positionX, p->plateau[i + 1][j - 1].positionY,
                                 p->plateau[i + 1][j - 1].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j - 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i][j + 1].positionX, p->plateau[i][j + 1].positionY,
                                 p->plateau[i][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i][j + 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j + 1].positionX, p->plateau[i - 1][j + 1].positionY,
                                 p->plateau[i - 1][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j + 1].positionY + TAILLE_CASE, couleurBat);

        al_draw_filled_rectangle(p->plateau[i][j + 2].positionX, p->plateau[i][j + 2].positionY,
                                 p->plateau[i][j + 2].positionX + TAILLE_CASE,
                                 p->plateau[i][j + 2].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j + 2].positionX, p->plateau[i + 1][j + 2].positionY,
                                 p->plateau[i + 1][j + 2].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j + 2].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j + 2].positionX, p->plateau[i - 1][j + 2].positionY,
                                 p->plateau[i - 1][j + 2].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j + 2].positionY + TAILLE_CASE, couleurBat);

        al_draw_filled_rectangle(p->plateau[i - 2][j + 2].positionX, p->plateau[i - 2][j + 2].positionY,
                                 p->plateau[i - 2][j + 2].positionX + TAILLE_CASE,
                                 p->plateau[i - 2][j + 2].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 2][j + 1].positionX, p->plateau[i - 2][j + 1].positionY,
                                 p->plateau[i - 2][j + 1].positionX + TAILLE_CASE,
                                 p->plateau[i - 2][j + 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 2][j].positionX, p->plateau[i - 2][j].positionY,
                                 p->plateau[i - 2][j].positionX + TAILLE_CASE,
                                 p->plateau[i - 2][j].positionY + TAILLE_CASE, couleurBat);

        al_draw_filled_rectangle(p->plateau[i - 2][j - 1].positionX, p->plateau[i - 2][j - 1].positionY,
                                 p->plateau[i - 2][j - 1].positionX + TAILLE_CASE,
                                 p->plateau[i - 2][j - 1].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 2][j - 2].positionX, p->plateau[i - 2][j - 2].positionY,
                                 p->plateau[i - 2][j - 2].positionX + TAILLE_CASE,
                                 p->plateau[i - 2][j - 2].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 2][j - 3].positionX, p->plateau[i - 2][j - 3].positionY,
                                 p->plateau[i - 2][j - 3].positionX + TAILLE_CASE,
                                 p->plateau[i - 2][j - 3].positionY + TAILLE_CASE, couleurBat);

        al_draw_filled_rectangle(p->plateau[i - 1][j - 3].positionX, p->plateau[i - 1][j - 3].positionY,
                                 p->plateau[i - 1][j - 3].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j - 3].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i - 1][j - 2].positionX, p->plateau[i - 1][j - 2].positionY,
                                 p->plateau[i - 1][j - 2].positionX + TAILLE_CASE,
                                 p->plateau[i - 1][j - 2].positionY + TAILLE_CASE, couleurBat);

        al_draw_filled_rectangle(p->plateau[i][j - 3].positionX, p->plateau[i][j - 3].positionY,
                                 p->plateau[i][j - 3].positionX + TAILLE_CASE,
                                 p->plateau[i][j - 3].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i][j - 2].positionX, p->plateau[i][j - 2].positionY,
                                 p->plateau[i][j - 2].positionX + TAILLE_CASE,
                                 p->plateau[i][j - 2].positionY + TAILLE_CASE, couleurBat);

        al_draw_filled_rectangle(p->plateau[i + 1][j - 3].positionX, p->plateau[i + 1][j - 3].positionY,
                                 p->plateau[i + 1][j - 3].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j - 3].positionY + TAILLE_CASE, couleurBat);
        al_draw_filled_rectangle(p->plateau[i + 1][j - 2].positionX, p->plateau[i + 1][j - 2].positionY,
                                 p->plateau[i + 1][j - 2].positionX + TAILLE_CASE,
                                 p->plateau[i + 1][j - 2].positionY + TAILLE_CASE, couleurBat);
    } else {
        al_draw_filled_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                 (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                 (float) p->plateau[i][j].positionY + TAILLE_CASE,
                                 couleurBat);
    }

}

//Type de batiment :
// Route = 0
// Habitation = 1
// Usine = 2
// Chateau d'eau = 3
// Caserne de pompier = 04