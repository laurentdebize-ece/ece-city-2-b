#include "include.h"


void dessinerniveau2(Principal *p) {

    al_clear_to_color(al_map_rgb(0, 0, 0));
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {

            switch (p->plateau[i][j].batiment.typeBatiments) {
                default:
                    break;
                case 1 : // une route
                    al_draw_filled_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                             (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                             (float) p->plateau[i][j].positionY + TAILLE_CASE,
                                             al_map_rgb(0, 0, 255));

                    break;
                case 2 : // un terrain vague
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 0) {
                        al_draw_bitmap(p->terrain_vague, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY,
                                       0);
                        al_draw_bitmap(p->terrain_vague, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY, 0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 1) { // cabane
                        al_draw_bitmap(p->cabane, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 2) { // maison
                        al_draw_bitmap(p->house, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 3) { // immeuble
                        al_draw_bitmap(p->immeuble, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 4) { // gratte-ciel
                        al_draw_bitmap(p->gratteciel, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }

                    break;
                case 3 : // une centrale

                    al_draw_bitmap(p->centrale, (float) p->plateau[i][j].positionX - 60,
                                   (float) p->plateau[i][j].positionY - 40, 0);
                    break;
            }
            al_draw_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                              (float) p->plateau[i][j].positionX + TAILLE_CASE,
                              (float) p->plateau[i][j].positionY + TAILLE_CASE, al_map_rgb(100, 100, 100), 1);
            for (int k = 0; k <= p->g->ordre; k++) {
                infoNiveaux(p, i, j, k);

            }
        }
        al_draw_filled_rectangle(950, 300, 1050, 350, al_map_rgb(200, 0, 0));
    }
    al_flip_display();
}

void infoNiveaux(Principal *p, int i, int j, int k) {
    /// PARTIE EAU
    /// on associe la bonne case du plateau avec son Sommet dans le graphe
    /// Permet d'accéder aux info du graphe
    if (p->niveau1 && p->plateau[i][j].batiment.typeBatiments == 4 && p->g->pSommet[k]->ligne == i &&
        p->g->pSommet[k]->colonne == j) {
        al_draw_textf(p->police2, al_map_rgb(0, 0, 0), p->plateau[i][j].positionX - 80,
                      p->plateau[i][j].positionY - 60,
                      0, " %d / 5000", p->g->pSommet[k]->construction.capaciteEau);
    }
    if (p->niveau1 && p->plateau[i][j].batiment.typeBatiments == 2 && p->g->pSommet[k]->ligne == i &&
        p->g->pSommet[k]->colonne == j) {
        /// booléen associé à chaque S du Graphe si true alors toute l'habitation est alimentée
        if (p->g->pSommet[k]->accesEauValide) {
            al_draw_textf(p->police2, al_map_rgb(0, 0, 0), p->plateau[i][j].positionX - 70,
                          p->plateau[i][j].positionY - 40, 0, "Eau : %d", p->g->pSommet[k]->nombreHabitant);
        }

        if (!p->g->pSommet[k]->accesEauValide) {
            al_draw_textf(p->police2, al_map_rgb(0, 0, 0), p->plateau[i][j].positionX - 70,
                          p->plateau[i][j].positionY - 40, 0, "Eau : %d",
                          p->g->pSommet[k]->nombreHabitant - p->g->pSommet[k]->habitantSansEau);
            /// nbr habitants alimentés en eau = nbr total - nbr sans eau
        }
    }



    /// PARTIE ELECTRICITE
    if (p->niveau2 && p->plateau[i][j].batiment.typeBatiments == 3 && p->g->pSommet[k]->ligne == i &&
        p->g->pSommet[k]->colonne == j) {
        al_draw_textf(p->police2, al_map_rgb(255, 255, 255), p->plateau[i][j].positionX - 80,
                      p->plateau[i][j].positionY - 70,
                      0, "%d / 5000", p->g->pSommet[k]->construction.capaciteElec);
    }
    if (p->niveau2 && p->plateau[i][j].batiment.typeBatiments == 2 && p->g->pSommet[k]->ligne == i &&
        p->g->pSommet[k]->colonne == j) {
        if (p->g->pSommet[k]->accesElecValide) {
            al_draw_textf(p->police2, al_map_rgb(255, 255, 255), (float)p->plateau[i][j].positionX - 70,
                          (float)p->plateau[i][j].positionY - 40, 0, "Elec : %d",
                           p->g->pSommet[k]->nombreHabitant);
        } else {
            al_draw_textf(p->police2, al_map_rgb(255, 255, 255), (float)p->plateau[i][j].positionX - 70,
                          (float)p->plateau[i][j].positionY - 40, 0, "Elec : 0");
        }
    }
}

void dessinerniveau1(Principal *p) {
    /// similaire  à dessiner matrice, mais avec seulement une partie des éléments
    al_clear_to_color(al_map_rgb(255, 255, 255));
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {

            switch (p->plateau[i][j].batiment.typeBatiments) {
                default:
                    break;
                case 1 : // une route
                    al_draw_filled_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                             (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                             (float) p->plateau[i][j].positionY + TAILLE_CASE,
                                             al_map_rgb(0, 0, 255));

                    break;
                case 2 : // un terrain vague
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 0) {
                        al_draw_bitmap(p->terrain_vague, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY,
                                       0);
                        al_draw_bitmap(p->terrain_vague, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY, 0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 1) { // cabane
                        al_draw_bitmap(p->cabane, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 2) { // maison
                        al_draw_bitmap(p->house, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 3) { // immeuble
                        al_draw_bitmap(p->immeuble, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 4) { // gratte-ciel
                        al_draw_bitmap(p->gratteciel, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);

                    }
                    break;

                case 4 : // un chateau d'eau

                    al_draw_bitmap(p->chateau, (float) p->plateau[i][j].positionX - 60,
                                   (float) p->plateau[i][j].positionY - 40, 0);
                    break;
            }
            al_draw_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                              (float) p->plateau[i][j].positionX + TAILLE_CASE,
                              (float) p->plateau[i][j].positionY + TAILLE_CASE, al_map_rgb(0, 0, 0), 1);
/// on affiche les informations des sommets du graphe
            for (int k = 0; k <= p->g->ordre; k++) {
                infoNiveaux(p, i, j, k);
            }
        }

        al_draw_filled_rectangle(950, 300, 1050, 350, al_map_rgb(200, 0, 0));

    }
    al_flip_display();
}
