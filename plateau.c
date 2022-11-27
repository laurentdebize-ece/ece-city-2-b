//
// Created by Louis Duvanel on 01/11/2022.
//
#include "include.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

void initialisationPlateau(Principal *p) {// Commence à x=10 & y=10

    int x = 10;
    int y = 10;
    p->ftexte = fopen("../matrice", "r");

    if (p->ftexte == NULL) {
        printf("Erreur d'ouverture\n");
    }

    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {

            fscanf(p->ftexte, " %d", &p->plateau[i][j].batiment.typeBatiments);
            p->plateau[i][j].positionX = x;
            p->plateau[i][j].positionY = y;
            p->plateau[i][j].occupee = -1;
            p->plateau[i][j].numeroSommetProximite = -1;
            p->plateau[i][j].adjacence = NULL;
            x += 20;
        }
        x = 10;
        y += 20;
    }
    fclose(p->ftexte);
} // Pour la matrice [LIGNES][COLONNES]


void afficherErreur(Principal *p) {
    al_draw_bitmap(p->tabfondjeu[4], 5, 300, 0);
}

void afficherErreur2(Principal *p) {
    al_draw_bitmap(p->tabfondjeu[5], 5, 300, 0);
}

void initialisationstructure(Principal *p) {
    p->occupe = 0;
    p->troisiemePageOutils = false;
    p->argentbanque = 500000;
    p->nbrhabitants = 0;
    p->choix = -1;
    p->capElec = 0;
    p->capEau = 0;
    p->niveau1 = false;
    p->niveau2 = false;
    p->chateauEau = (int *) malloc(MAX_MAISON * sizeof(int));//Numero des chateaux d'eau
    p->centraleElectrique = (int *) malloc(MAX_MAISON * sizeof(int));
    p->compteurChateau = 0;
    p->compteurCentrale = 0;
    for (int i = 0; i < MAX_MAISON; i++) {
        p->chateauEau[i] = -1;
        p->centraleElectrique[i] = -1;
    }
    p->compteurChateau = 0;
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {
            p->plateau[i][j].canalisation = -1;
            p->plateau[i][j].route = 0;
            p->plateau[i][j].construction = -1;
            p->plateau[i][j].batiment.stadeDeConstruction = 0;
            for (int k = 0; k < 12; k++) {
                p->plateau[i][j].batiment.tab[k].x = -1;
                p->plateau[i][j].batiment.tab[k].y = -1;
            }
        }
    }

}


void miseAJourDuNombreHabitantParSommet(Principal *p) {
    p->nbrhabitants = 0;
    for (int i = 1; i <= p->g->ordre; i++) {
        if (p->g->pSommet[i]->construction.typeBatiments == 2) {
            switch (p->plateau[p->g->pSommet[i]->ligne][p->g->pSommet[i]->colonne].batiment.stadeDeConstruction) {
                case -1 :
                    p->g->pSommet[i]->nombreHabitant = 0;
                    break;
                case 0 :
                    p->g->pSommet[i]->nombreHabitant = 0;//Terrain vague
                    break;
                case 1 :
                    p->g->pSommet[i]->nombreHabitant = 10;//Cabane
                    break;
                case 2 :
                    p->g->pSommet[i]->nombreHabitant = 50;//Maison
                    break;
                case 3 :
                    p->g->pSommet[i]->nombreHabitant = 100;//Immeuble
                    break;
                case 4 :
                    p->g->pSommet[i]->nombreHabitant = 1000;//Maison
                    break;
                default:
                    p->g->pSommet[i]->nombreHabitant = p->g->pSommet[i]->nombreHabitant;
                    break;
            }
            p->nbrhabitants = p->g->pSommet[i]->nombreHabitant + p->nbrhabitants;
        } else {
            p->g->pSommet[i]->nombreHabitant = 0;
        }
    }
}

bool detectionViabilite(Principal *p, int i, int j, int k) {
    bool viable = false;
    if (p->g->pSommet[k]->habitantSansEau > 0 || p->g->pSommet[k]->habitantSansElec > 0) {
        viable = false;
    }
    if (p->g->pSommet[k]->accesEauValide && p->g->pSommet[k]->accesElecValide) {
        viable = true;
    }
    return viable;
}

void agrandissementBatiment(Principal *p) {
    bool viable = false;
    for (int i = 0; i < LIGNE; i++) {
        for (int j = 0; j < COLONNE; j++) {
            for (int k = 0; k <= p->g->ordre; k++) {
                if (p->modeDeJeu == 0) {
                    /// Le mode communiste : On fait evoluer le batiment si la capacité
                    /// avant amélioration est vérifiée et on vole l'eau aux voisins.
                    /// on regarde où il y a une maison, et à quel pSommet elle correspond
                    if (p->plateau[i][j].batiment.typeBatiments == 2 && p->g->pSommet[k]->colonne == j &&
                        p->g->pSommet[k]->ligne == i) {

                        printf("On trouve le sommet %d de stade : %d\n", p->g->pSommet[k]->numero,
                               p->g->pSommet[k]->stadeConstruction);


                        viable = detectionViabilite(p, i, j, k);

                        /// augmentation du STADE DE CONSTRUCTION
                        switch (p->plateau[i][j].batiment.stadeDeConstruction) {
                            case -1:
                                if (viable) {
                                    p->plateau[i][j].batiment.stadeDeConstruction =
                                            p->plateau[i][j].batiment.stadeDeConstruction + 1;
                                }
                                break;
                            case 0 :
                                if (viable &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroCentraleAssociee]->construction.capaciteElec >=
                                    10 &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroChateauAssociee]->construction.capaciteEau >=
                                    10) {
                                    p->plateau[i][j].batiment.stadeDeConstruction =
                                            p->plateau[i][j].batiment.stadeDeConstruction + 1;
                                } else if (!viable) {
                                    p->plateau[i][j].batiment.stadeDeConstruction -= 1;
                                }
                                break;
                            case 1 :
                                if (viable &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroCentraleAssociee]->construction.capaciteElec >=
                                    40 &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroChateauAssociee]->construction.capaciteEau >=
                                    40) {
                                    p->plateau[i][j].batiment.stadeDeConstruction =
                                            p->plateau[i][j].batiment.stadeDeConstruction + 1;
                                } else if (!viable) {
                                    p->plateau[i][j].batiment.stadeDeConstruction -= 1;
                                }
                                break;
                            case 2 :
                                if (viable &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroCentraleAssociee]->construction.capaciteElec >=
                                    50 &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroChateauAssociee]->construction.capaciteEau >=
                                    50) {
                                    p->plateau[i][j].batiment.stadeDeConstruction =
                                            p->plateau[i][j].batiment.stadeDeConstruction + 1;
                                } else if (!viable) {
                                    p->plateau[i][j].batiment.stadeDeConstruction -= 1;
                                }
                                break;
                            case 3 :
                                if (viable &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroCentraleAssociee]->construction.capaciteElec >=
                                    900 &&
                                    p->g->pSommet[p->plateau[i][j].batiment.numeroChateauAssociee]->construction.capaciteEau >=
                                    900) {
                                    p->plateau[i][j].batiment.stadeDeConstruction =
                                            p->plateau[i][j].batiment.stadeDeConstruction + 1;
                                } else if (!viable) {
                                    p->plateau[i][j].batiment.stadeDeConstruction -= 1;
                                }
                                break;
                            case 4 :
                                if (!viable) {
                                    p->plateau[i][j].batiment.stadeDeConstruction -= 1;
                                }
                                break;
                        }
                        viable = false;
                    }
                } else if (p->modeDeJeu == 1) { ///  Mode capitaliste
                    if (p->plateau[i][j].batiment.typeBatiments == 2 && p->g->pSommet[k]->colonne == j &&
                        p->g->pSommet[k]->ligne == i) {

                        viable = detectionViabilite(p, i, j, k);

                        /// augmentation du STADE DE CONSTRUCTION

                        if (viable) {
                            if (p->plateau[i][j].batiment.stadeDeConstruction < 4) {
                                p->plateau[i][j].batiment.stadeDeConstruction =
                                        p->plateau[i][j].batiment.stadeDeConstruction + 1;
                            }
                        } else if (!viable) {
                            if (p->plateau[i][j].batiment.stadeDeConstruction > -1) {
                                p->plateau[i][j].batiment.stadeDeConstruction -= 1;
                            }
                        }
                        viable = false;
                    }


                }

                // Ensuite faudra juste mettre l'affichage a jour avec la forme de chaque bat
                // en fonction de leur niveau qu'il ait changé ou nn.

            }
        }
    }
    miseAJourDuNombreHabitantParSommet(p);
}

void dessinerMatrice(Principal *p) {

    bool affichageterrain = false;
    bool affichagemaison = false;
    bool affichagecentrale = false;
    bool affichagecabane = false;
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {
            if (p->plateau[i][j].occupee == -1) {
                al_draw_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                  (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                  (float) p->plateau[i][j].positionY + TAILLE_CASE, al_map_rgba(0, 0, 0, 200), 1);
                al_draw_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                  (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                  (float) p->plateau[i][j].positionY + TAILLE_CASE, al_map_rgba(0, 0, 0, 200), 1);
            }
            switch (p->plateau[i][j].batiment.typeBatiments) {

                case 1 : /// une route

                    al_draw_filled_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                             (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                             (float) p->plateau[i][j].positionY + TAILLE_CASE, al_map_rgb(0, 0, 0));

                    break;
                case 2 : /// un terrain vague

                    if (p->plateau[i][j].batiment.stadeDeConstruction == -1) {
                        al_draw_bitmap(p->ruine, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY,
                                       0);
                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 0) {
                        al_draw_bitmap(p->terrain_vague, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY,
                                       0);
                        al_draw_bitmap(p->terrain_vague, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i - 1][j].positionY, 0);
                        affichageterrain = true;
                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 1) { // cabane
                        al_draw_bitmap(p->cabane, (float) p->plateau[i][j].positionX - 40,
                                       (float) p->plateau[i][j].positionY - 40,
                                       0);
                        affichagecabane = true;
                    }
                    if (p->plateau[i][j].batiment.stadeDeConstruction == 2) { // maison
                        al_draw_bitmap(p->house, (float) p->plateau[i][j].positionX - 20,
                                       (float) p->plateau[i][j].positionY - 20,
                                       0);
                        affichagemaison = true;

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
                case 3 : /// une centrale

                    al_draw_bitmap(p->centrale, (float) p->plateau[i][j].positionX - 60,
                                   (float) p->plateau[i][j].positionY - 40, 0);

                    break;

                case 4 : /// un chateau d'eau

                    al_draw_bitmap(p->chateau, (float) p->plateau[i][j].positionX - 60,
                                   (float) p->plateau[i][j].positionY - 40, 0);
                    break;
                case 5 :/// une caserne de pompier

                    al_draw_bitmap(p->caserne, (float) p->plateau[i][j].positionX - 75,
                                   (float) p->plateau[i][j].positionY-75, 0);
                    break;
                case 6 :
                    al_draw_bitmap(p->library,(float) p->plateau[i][j].positionX - 30,
                                   (float) p->plateau[i][j].positionY - 35, 0);
                    break;

                case 7 :
                    al_draw_bitmap(p->school,(float) p->plateau[i][j].positionX - 60,
                                   (float) p->plateau[i][j].positionY - 58, 0);
                    break;
                case 8 :
                    al_draw_bitmap(p->parc,(float) p->plateau[i][j].positionX -10,
                    (float) p->plateau[i][j].positionY -25, 0);
                    break;
            }
        }
    }

}

int barreInteractifArgent(Principal *p) {
    int evolution = 0;
    if (p->argentbanque == 500000) {
        evolution = 0;
    } else if (490000 <= p->argentbanque && p->argentbanque < 500000) {
        evolution = 1;
    } else if (390000 <= p->argentbanque && p->argentbanque < 490000) {
        evolution = 2;
    } else if (380500 <= p->argentbanque && p->argentbanque < 390000) {
        evolution = 3;
    } else if (300000 <= p->argentbanque && p->argentbanque < 385000) {
        evolution = 4;
    } else if (250000 <= p->argentbanque && p->argentbanque < 300000) {
        evolution = 5;
    } else if (200000 <= p->argentbanque && p->argentbanque < 250000) {
        evolution = 6;
    } else if (100000 <= p->argentbanque && p->argentbanque < 200000) {
        evolution = 7;
    } else if (80000 <= p->argentbanque && p->argentbanque < 100000) {
        evolution = 8;
    }

    return evolution;
}

int barreInteractifPopulation(Principal *p) {
    int population = 0;
    if (p->nbrhabitants == 0) {
        population = 0;
    } else if (10 <= p->nbrhabitants && p->nbrhabitants < 30) {
        population = 1;
    } else if (30 <= p->nbrhabitants && p->nbrhabitants < 100) {
        population = 2;
    } else if (100 <= p->nbrhabitants && p->nbrhabitants < 800) {
        population = 3;
    } else if (800 <= p->nbrhabitants && p->nbrhabitants < 2500) {
        population = 4;
    } else if (2500 <= p->nbrhabitants && p->nbrhabitants < 3500) {
        population = 5;
    } else if (p->nbrhabitants >= 3500) {
        population = 6;
    }

    return population;
}

void initialisationImagesArgent(Principal *p) {
    p->barreArgent[0] = al_load_bitmap("../images/argent.png");
    p->barreArgent[1] = al_load_bitmap("../images/argent2.png");
    p->barreArgent[2] = al_load_bitmap("../images/argent3.png");
    p->barreArgent[3] = al_load_bitmap("../images/argent4.png");
    p->barreArgent[4] = al_load_bitmap("../images/argent5.png");
    p->barreArgent[5] = al_load_bitmap("../images/argent6.png");
    p->barreArgent[6] = al_load_bitmap("../images/argent7.png");
    p->barreArgent[7] = al_load_bitmap("../images/argent8.png");
    p->barreArgent[8] = al_load_bitmap("../images/argent9.png");
    p->tabfondjeu[0] = al_load_bitmap("../images/fond_jeu3.png");
    p->tabfondjeu[1] = al_load_bitmap("../images/fond_jeu2.png");
    p->tabfondjeu[2] = al_load_bitmap("../images/fond_jeu1.png");
    p->tabfondjeu[3] = al_load_bitmap("../images/fond_jeu4.png");
    p->tabfondjeu[4] = al_load_bitmap("../images/erreur.png");
    p->tabfondjeu[5] = al_load_bitmap("../images/erreur2.png");
}

void initialisationImagesPopulation(Principal *p) {
    p->barrePopulation[0] = al_load_bitmap("../images/popu1.png");
    p->barrePopulation[1] = al_load_bitmap("../images/popu2.png");
    p->barrePopulation[2] = al_load_bitmap("../images/popu3.png");
    p->barrePopulation[3] = al_load_bitmap("../images/popu4.png");
    p->barrePopulation[4] = al_load_bitmap("../images/popu5.png");
    p->barrePopulation[5] = al_load_bitmap("../images/popu6.png");
    p->barrePopulation[6] = al_load_bitmap("../images/popu7.png");

}

void affichageInformation(Principal *p, ALLEGRO_FONT *police) {
    int evolution = 0;
    evolution = barreInteractifArgent(p);
    // TIMER
    al_draw_textf(p->police2, al_map_rgb(255, 255, 255), 1100, 320, 0,
                  "%d : %d ", p->mintimer, p->sectimer);
    // BARRE HABITANTS
    al_draw_bitmap(p->barrePopulation[barreInteractifPopulation(p)], 905, 200, 0);
    al_draw_textf(police, al_map_rgb(255, 255, 255), 950, 285, 0,
                  "Nombre d'habitant ");
    // NBR HABITANTS
    al_draw_textf(police, al_map_rgb(255, 255, 255), 1100, 285, 0, " %d ", p->nbrhabitants);
    // BOUTON QUITTER
    if (!p->sourisBoutonQuitter) {
        al_draw_bitmap(p->bouton_quitter, 970, 600, 0);
    }
    switch (evolution) {
        case 8:
            al_draw_bitmap(p->barreArgent[evolution], 910, 129, 0);
            break;
        case 7:
            al_draw_bitmap(p->barreArgent[evolution], 920, 143, 0);
            break;
        case 6:
            al_draw_bitmap(p->barreArgent[evolution], 920, 143, 0);
            break;

        default:
            al_draw_bitmap(p->barreArgent[evolution], 920, 150, 0);
            break;

    }
    al_draw_textf(p->police2, al_map_rgb(255, 255, 255), 1010, 150, 0,
                  " %d $", p->argentbanque);

    /// MODE COMMUNISTE OU CAPITALISTE
    switch (p->modeDeJeu) {
        case 1 :
            al_draw_textf(p->police2, al_map_rgb(0, 0, 0), 950, 650, 0, "MODE COMMUNISTE");
            break;

        case 2 :
            al_draw_textf(p->police2, al_map_rgb(0, 0, 0), 950, 650, 0, "MODE CAPITALISTE");
            break;
    }
}

void affichageInteractifSouris(Principal *p, int x, int y) {

    ALLEGRO_COLOR couleurCaseSouris;

    dessinerMatrice(p);
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {
            if (p->plateau[i][j].positionX <= x && x <= p->plateau[i][j].positionX + TAILLE_CASE &&
                p->plateau[i][j].positionY <= y && y <= p->plateau[i][j].positionY + TAILLE_CASE) {
                switch (p->choix) {
                    case -1 ://Rien
                        couleurCaseSouris = al_map_rgb(255, 255, 255);
                        break;
                    case 1 ://Route
                        al_draw_filled_rectangle((float) p->plateau[i][j].positionX, (float) p->plateau[i][j].positionY,
                                                 (float) p->plateau[i][j].positionX + TAILLE_CASE,
                                                 (float) p->plateau[i][j].positionY + TAILLE_CASE,
                                                 al_map_rgba(0, 0, 0, 100));
                        break;
                    case 2 ://Maison
                        couleurCaseSouris = al_map_rgba(0, 255,0,20);
                        dessinFantomeMaison(p, i, j, couleurCaseSouris);
                        break;
                    case 3 ://Usine elec
                        couleurCaseSouris = al_map_rgba(255, 255, 0, 20);
                        dessinFantomeAutreConstruction(p, couleurCaseSouris, i, j);
                        break;
                    case 4 ://Eau
                        couleurCaseSouris = al_map_rgba(0, 0, 255, 20);
                        dessinFantomeAutreConstruction(p, couleurCaseSouris, i, j);
                        break;
                    case 5 ://Caserne
                        couleurCaseSouris = al_map_rgba(255, 0, 0, 20);
                        dessinFantomeAutreConstruction(p, couleurCaseSouris, i, j);
                        break;
                    case 6 ://Bibliotheque
                        couleurCaseSouris = al_map_rgba(88,41, 0, 20);
                        dessinFantomeMaison(p, i, j, couleurCaseSouris);
                        break;
                    case 7 :
                        couleurCaseSouris = al_map_rgba(40, 103, 91, 20);
                        dessinFantomeAutreConstruction(p, couleurCaseSouris, i,j);
                        break;
                    case 8 :
                        dessinFantomeParc(p,i,j);
                        break;
                }
            }
        }
    }
}


void initialisationPrincipale(Principal *p){
    p->police2 = al_load_ttf_font("../Bangers-Regular.ttf", 25, ALLEGRO_ALIGN_CENTER);
    p->sectimer = 0;
    p->mintimer = 0;
    p->erreur = false;
    p->erreur2 = false;
    p->sourisBoutonQuitter = false;
    p->quitterpartie = false;
    p->secondePageOutils = false;
    p->sauvegarder = false;
    p->caserne = al_load_bitmap("../images/caserne.png");
    p->chateau = al_load_bitmap("../images/chateau_eau.png");
    p->terrain_vague = al_load_bitmap("../images/terrain_vide.png");
    p->cabane = al_load_bitmap("../cabane.png");
    p->parc = al_load_bitmap("../images/jeu.png");
    p->house = al_load_bitmap("../images/house.png");
    p->gratteciel = al_load_bitmap("../images/gratte_ciel.png");
    p->centrale = al_load_bitmap("../centrale.tiff");
    p->library = al_load_bitmap("../images/biblio.png");
    p->school = al_load_bitmap("../images/school.png");
    p->parametre.police = al_load_ttf_font("../Bangers-Regular.ttf", 16, ALLEGRO_ALIGN_CENTER);
    p->imageBoiteOutils = al_load_bitmap("../images/outils.png");
    p->immeuble = al_load_bitmap("../images/immeuble.png");
    p->ruine = al_load_bitmap("../images/ruine.png");
    p->bouton_quitter = al_load_bitmap("../images/bouton_quitter.png");
    p->bouton_quitter2 = al_load_bitmap("../images/bouton_quitter2.png");
    p->timer = al_create_timer(1);
    p->parametre.display = al_create_display(1200, 800);
}

void affichageMatrice(Principal *p) {

    assert(al_init());
    assert(al_init_image_addon());
    assert(al_init_primitives_addon());
    al_init_font_addon();
    assert(al_init_ttf_addon());
    al_install_keyboard();
    al_install_mouse();
    assert(al_install_audio());
    assert(al_init_acodec_addon());


    ///////////// initialisation des variables   ////////////////////////
    initialisationPrincipale(p);
    int x, y;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_COLOR couleur;
    ALLEGRO_BITMAP *boite_outils = NULL;
    ALLEGRO_BITMAP *horloge = NULL;
    ALLEGRO_BITMAP *pause = NULL;
    fflush(stdin);


    int chronometreAmeliorationBatiment = 0;
    int chronometreaffichage = 0;
    bool fin = false;
    bool affichageinfo = true;
    bool sourisDansMatrice = false;
    bool boiteOutilOpen = false;
    bool sourisSurBoiteOutils = false;
    bool jeuEnPause = false;

    boite_outils = al_load_bitmap("../images/nouvelle_boite_outils.png");
    horloge = al_load_bitmap("../images/clock.png");
    pause = al_load_bitmap("../images/PAUSE.png");

    ///verifications

    queue = al_create_event_queue();
    couleur = al_map_rgb(70, 170, 70);

    timer = al_create_timer(0.1);

    al_clear_to_color(couleur);
    al_set_window_position(p->parametre.display, 0, 0);
    p->g = creerGraphe(p);
    p->g->ordre = 0;
    dessinerMatrice(p);
    initialisationImageBoiteOutils(p);
    initialisationImagesArgent(p);
    initialisationImagesPopulation(p);


    al_register_event_source(queue, al_get_display_event_source(p->parametre.display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(p->timer));


    al_start_timer(timer);
    al_start_timer(p->timer);
    al_flip_display();

    while (fin == false) {


        al_wait_for_event(queue, &p->parametre.event);

        switch (p->parametre.event.type) {
            case ALLEGRO_EVENT_KEY_UP :
                if (p->parametre.event.keyboard.keycode == ALLEGRO_KEY_P) {
                    jeuEnPause = true;
                    al_draw_bitmap(pause, 510, 300, 0);
                    al_flip_display();
                    while (jeuEnPause == true) {
                        al_wait_for_event(queue, &p->parametre.event);
                        if (p->parametre.event.type == ALLEGRO_EVENT_KEY_UP &&
                            p->parametre.event.keyboard.keycode == ALLEGRO_KEY_P) {
                            jeuEnPause = false;
                        }
                    }
                }
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                if (p->plateau[0][0].positionX <= p->parametre.event.mouse.x &&
                    p->parametre.event.mouse.x <= p->plateau[0][44].positionX + TAILLE_CASE &&
                    p->plateau[0][0].positionY <= p->parametre.event.mouse.y &&
                    p->parametre.event.mouse.y <= p->plateau[34][0].positionY + TAILLE_CASE &&
                    boiteOutilOpen == false) {

                    x = p->parametre.event.mouse.x;
                    y = p->parametre.event.mouse.y;
                    sourisDansMatrice = true;

                } else {
                    sourisDansMatrice = false;
                }

                if (BOITE_OUTIL_X <= p->parametre.event.mouse.x && p->parametre.event.mouse.x <= BOITE_OUTIL_X2 &&
                    BOITE_OUTIL_Y <= p->parametre.event.mouse.y && p->parametre.event.mouse.y <= BOITE_OUTIL_Y2) {
                    sourisSurBoiteOutils = true;
                } else {
                    sourisSurBoiteOutils = false;
                }
                if (970 <= p->parametre.event.mouse.x && p->parametre.event.mouse.x <= 970 + 110 &&
                    600 <= p->parametre.event.mouse.y && p->parametre.event.mouse.y <= 650) {
                    p->sourisBoutonQuitter = true;
                } else {
                    p->sourisBoutonQuitter = false;
                }

                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE :
                fin = true;
                p->leJeuEstLance = false;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: // Attention file prioritaire à la EVENT_MOUSE_BUTTON_DOWN

                if (sourisDansMatrice) {

                    x = p->parametre.event.mouse.x;
                    y = p->parametre.event.mouse.y;


                    if (p->enCoursdeConstruction == true) {
                        placementconstruction(p, x, y);
                        p->enCoursdeConstruction = false;
                    }

                    // on initialise la capacité si on place une centrale ou un château d'eau
                    if (p->choix == 3) {
                        p->capElec += 5000;
                    }
                    if (p->choix == 4) {
                        p->capEau += 5000;
                    }
                    p->choix = -1;
                }
                // si on clique ici on passe au niveau -1
                if (970 <= p->parametre.event.mouse.x && p->parametre.event.mouse.x <= 970 + 150 &&
                    480 <= p->parametre.event.mouse.y && p->parametre.event.mouse.y <= 480 + 70) {
                    p->niveau1 = true;
                    affichageinfo = false;
                }
                    /// si on clique ici on passe au niveau -1
                    if (p->niveau1 == true && x >= 950 && x <= 1050 && y >= 300 && y <= 350) {
                        p->niveau1 = false;
                        boiteOutilOpen = false;
                        affichageinfo = true;
                    }
                    if (p->niveau2 == true && x >= 950 && x <= 1050 && y >= 300 && y <= 350) {
                        p->niveau2 = false;
                        boiteOutilOpen = false;
                        affichageinfo = true;
                    }

                    /// QUITTER PARTIE : on regarde si le joueur a décidé de sauvegarder ou quitter
                    if (p->quitterpartie && 400 <= p->parametre.event.mouse.x && p->parametre.event.mouse.x <= 760 &&
                        290 <= p->parametre.event.mouse.y && p->parametre.event.mouse.y <= 410) {
                        p->sauvegarder = true; // on est sur la page quitter mais le joueur veut sauvegarder la partie
                    }
                    if (p->quitterpartie && 400 <= p->parametre.event.mouse.x && p->parametre.event.mouse.x <= 760 &&
                        470 <= p->parametre.event.mouse.y && p->parametre.event.mouse.y <= 600) {
                        fin = true;
                        //affichageinfo = true ;
                    }
                    /// appui sur le bouton QUIT
                    if (p->sourisBoutonQuitter) {
                        p->quitterpartie = true;
                        affichageinfo = false;
                    }
                    x = p->parametre.event.mouse.x;
                    y = p->parametre.event.mouse.y;

                    if (BOITE_OUTIL_X <= p->parametre.event.mouse.x && p->parametre.event.mouse.x <= BOITE_OUTIL_X2 &&
                        BOITE_OUTIL_Y <= p->parametre.event.mouse.y && p->parametre.event.mouse.y <= BOITE_OUTIL_Y2) {
                        boiteOutilOpen = true;
                    } else if (p->secondePageOutils || p->troisiemePageOutils ||
                               RECT_TRANSI_X <= x && x <= RECT_TRANSI_X2 && RECT_TRANSI_Y <= y && y <= RECT_TRANSI_Y2) {
                        boiteOutilOpen = true;
                    } else if (RECT_TRANSI_2_X <= x && x <= RECT_TRANSI_2_X2 && RECT_TRANSI_2_Y <= y &&
                               y <= RECT_TRANSI_2_Y2) {
                        boiteOutilOpen = true;
                    } else {
                        boiteOutilOpen = false;
                    }

                    break;
                    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                        if (boiteOutilOpen) {
                            x = p->parametre.event.mouse.x;
                            y = p->parametre.event.mouse.y;
                            ///Cas ou on clique pour aller a la deuxieme page
                            if (p->secondePageOutils == false && p->troisiemePageOutils == false &&
                                RECT_TRANSI_X <= x &&
                                x <= RECT_TRANSI_X2 && RECT_TRANSI_Y <= y && y <= RECT_TRANSI_Y2) {
                                p->secondePageOutils = true;
                                printf("Ouverture deuxieme boite\n");
                            }
                                ///Cas ou on veut aller a la troisieme page
                            else if (p->secondePageOutils == true && p->troisiemePageOutils == false &&
                                     RECT_TRANSI_X <= x &&
                                     x <= RECT_TRANSI_X2 && RECT_TRANSI_Y <= y && y <= RECT_TRANSI_Y2) {
                                p->troisiemePageOutils = true;
                                p->secondePageOutils = false;
                            }
                                ///Cas ou on veut quitter la troisieme page
                            else if (p->secondePageOutils == false && p->troisiemePageOutils == true &&
                                     RECT_TRANSI_2_X <= x &&
                                     x <= RECT_TRANSI_2_X2 && RECT_TRANSI_2_Y <= y && y <= RECT_TRANSI_2_Y2) {
                                p->secondePageOutils = true;
                                p->troisiemePageOutils = false;
                            } else if (p->secondePageOutils && RECT_TRANSI_2_X <= x && x <= RECT_TRANSI_2_X2 &&
                                       RECT_TRANSI_2_Y <= y && y <= RECT_TRANSI_2_Y2) {
                                p->secondePageOutils = false;
                                reperageCliqueCaseOutils(p, x, y, &boiteOutilOpen);
                            } else {
                                reperageCliqueCaseOutils(p, x, y, &boiteOutilOpen);
                                boiteOutilOpen=false;
                                p->secondePageOutils = false;
                                p->troisiemePageOutils = false;
                            }
                        }
                    if (p->erreur) {
                        p->erreur = false;
                    }
                    if (p->erreur2) {
                        p->erreur2 = false;
                    }
                    break;
                    case ALLEGRO_EVENT_TIMER:

                        if (chronometreaffichage >= 0 && chronometreaffichage < 300) {
                            al_draw_bitmap(p->tabfondjeu[0], 0, 0, 0);
                        }
                    if (chronometreaffichage >= 300 && chronometreaffichage < 450) {
                        al_draw_bitmap(p->tabfondjeu[1], 0, 0, 0);
                    }
                    if (chronometreaffichage >= 450 && chronometreaffichage < 600) {
                        al_draw_bitmap(p->tabfondjeu[2], 0, 0, 0);
                    }
                    if (chronometreaffichage >= 600 && chronometreaffichage < 750) {
                        al_draw_bitmap(p->tabfondjeu[3], 0, 0, 0);
                    }
                    chronometreaffichage++;
                    if (chronometreaffichage >= 750) {
                        chronometreaffichage = 0;
                    }
                    if (sourisDansMatrice) {
                        affichageInteractifSouris(p, x, y);
                    }
                    /// AGRANDISSEMENT DES BATIMENTS TOUTES LES 15 SEC
                    if (chronometreAmeliorationBatiment == 150) {
                        for (int i = 0; i < p->compteurChateau; i++) {
                            bfs(p, p->chateauEau[i]);
                        }
                        for (int i = 0; i < p->compteurCentrale; i++) {
                            bfsElectricite(p, p->centraleElectrique[i]);
                        }
                        impots(p);
                        agrandissementBatiment(p);
                        chronometreAmeliorationBatiment = 0;
                    }
                    chronometreAmeliorationBatiment++;
                    dessinerMatrice(p);

                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    al_draw_bitmap(boite_outils, BOITE_OUTIL_X, BOITE_OUTIL_Y - 30, 0);
                    if (sourisSurBoiteOutils) {
                        al_draw_rounded_rectangle(BOITE_OUTIL_X + 5, BOITE_OUTIL_Y - 20, BOITE_OUTIL_X2 + 15,
                                                  BOITE_OUTIL_Y2 + 20, 4, 5,
                                                  al_map_rgb(0, 0, 0), 3);
                    }
                    // effet d'agrandissement quand on passe la souris sur QUIT
                    if (p->sourisBoutonQuitter) {
                        al_draw_bitmap(p->bouton_quitter2, 985, 595, 0);
                    }
                    if (p->niveau1) {
                        dessinerniveau1(p);
                        boiteOutilOpen = false;
                        p->secondePageOutils = false;
                        p->troisiemePageOutils = false;
                    }
                    if (p->niveau2) {
                        dessinerniveau2(p);
                        boiteOutilOpen = false;
                    }
                    if (boiteOutilOpen) {
                        menuBoiteOutils(p);
                    }
                    if (p->secondePageOutils) {
                        menuBoiteOutils2(p);
                    }
                    if (p->troisiemePageOutils) {
                        menuBoiteOutils3(p);
                    }

                    if (p->quitterpartie) {
                        fin = quitter(p);
                    }
                    if (p->erreur) {
                        afficherErreur(p);
                    }
                    if (p->erreur2) {
                        afficherErreur2(p);
                    }
                    if (affichageinfo) {
                        comptetimer(p, p->parametre.event);
                        al_draw_bitmap(horloge, 980, 300, 0);
                        affichageInformation(p, p->parametre.police);
                    }
                    al_flip_display();
                    break;
            }
        }
        al_destroy_bitmap(boite_outils);
        //al_destroy_bitmap(horloge);
        al_destroy_bitmap(pause);
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        liberer(p);
}

void impots(Principal *p) {
    p->argentbanque += p->nbrhabitants * 10;
}


/// fonction appellée à la fin du code pour libérer la mémoire
void liberer(Principal *p) {
    al_destroy_timer(p->timer);
    al_destroy_font(p->parametre.police);
    al_destroy_font(p->police2);
    al_destroy_display(p->parametre.display);
    al_destroy_bitmap(p->centrale);
    al_destroy_bitmap(p->terrain_vague);
    al_destroy_bitmap(p->house);
    al_destroy_bitmap(p->cabane);
    al_destroy_bitmap(p->caserne);
    al_destroy_bitmap(p->chateau);
    al_destroy_bitmap(p->gratteciel);
    al_destroy_bitmap(p->immeuble);
    for (int i = 0; i < 8; i++) {
        al_destroy_bitmap(p->barreArgent[i]);
    }
    for (int i = 0; i < 6; i++) {
        al_destroy_bitmap(p->tabfondjeu[i]);
    }
    for (int i = 0; i < 6; i++) {
        al_destroy_bitmap(p->barrePopulation[i]);
    }
    for (int i = 0; i < 13; i++) {
        if(i!=6) {
            al_destroy_bitmap(p->affichageBoiteOutils[i]);
        }
    }
    al_destroy_bitmap(p->imageBoiteOutils);
    al_destroy_bitmap(p->bouton_quitter);
    al_destroy_bitmap(p->bouton_quitter2);
    al_destroy_bitmap(p->quitter);
    free(p->chateauEau);
    free(p->centraleElectrique);
}

void comptetimer(Principal *p, ALLEGRO_EVENT event) {
    if (event.timer.source == p->timer) {
        if (p->sectimer < 60) {
            p->sectimer++; // compte les secondes
        }
        if (p->sectimer == 60) {
            p->sectimer = 0; // on réinitialise les secondes
            p->mintimer++; // et ajoute une minute

        }
    }
}

void placementIndiceDeProximite(Principal *p, int i, int j) {
    if (p->choix != 2) {
        p->plateau[i - 2][j - 3].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 2][j - 2].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 2][j - 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 2][j].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 2][j + 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 2][j + 2].numeroSommetProximite = p->g->ordre;

        p->plateau[i + 1][j - 3].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j - 2].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j - 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j + 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j + 2].numeroSommetProximite = p->g->ordre;

        p->plateau[i - 1][j - 3].numeroSommetProximite = p->g->ordre;
        p->plateau[i][j - 3].numeroSommetProximite = p->g->ordre;

        p->plateau[i][j + 2].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 1][j + 2].numeroSommetProximite = p->g->ordre;
    } else {
        p->plateau[i - 1][j].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j].numeroSommetProximite = p->g->ordre;
        p->plateau[i][j + 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i][j - 1].numeroSommetProximite = p->g->ordre;

        p->plateau[i - 1][j + 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i - 1][j - 1].numeroSommetProximite = p->g->ordre;

        p->plateau[i + 1][j + 1].numeroSommetProximite = p->g->ordre;
        p->plateau[i + 1][j - 1].numeroSommetProximite = p->g->ordre;
    }

}
/// s'active dès que l'on place une constructions via la boite à outils
void placementconstruction(Principal *p, int x, int y) {
    bool variableOnFaitLeBFS = false;

    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 45; j++) {
            if (p->plateau[i][j].positionX <= x && x <= p->plateau[i][j].positionX + TAILLE_CASE &&
                p->plateau[i][j].positionY <= y && y <= p->plateau[i][j].positionY + TAILLE_CASE) {
                /// cette fonction définie les cases sur lesquelles la construction sera
                /// selon le type du batiment sélectionnés
                if (p->plateau[i][j].occupee != -1) {
                    p->erreur = true;
                } else if ((p->choix == 3 || p->choix == 4 || p->choix == 5) && (i < 2 || j < 3 || i > 33 || j > 42)) {
                    p->erreur2 = true;
                } else if (p->choix == 2 && (i == 0 || j == 0 || i == 34 || j == 44)) {
                    p->erreur2 = true;
                } else if ((p->choix == 2) && (p->plateau[i + 1][j + 1].occupee != -1 ||
                                              p->plateau[i + 1][j].occupee != -1
                                              || p->plateau[i + 1][j - 1].occupee != -1 ||
                                              p->plateau[i][j - 1].occupee != -1 ||
                                              p->plateau[i][j + 1].occupee != -1 ||
                                              p->plateau[i - 1][j - 1].occupee != -1 ||
                                              p->plateau[i - 1][j].occupee != -1 ||
                                              p->plateau[i - 1][j + 1].occupee != -1)) {
                    p->erreur = true;
                } else if ((p->choix == 3 || p->choix == 4 || p->choix == 5) &&
                           ((p->plateau[i + 1][j + 1].occupee != -1 || p->plateau[i + 1][j].occupee != -1 ||
                             p->plateau[i + 1][j - 1].occupee != -1 || p->plateau[i][j - 1].occupee != -1 ||
                             p->plateau[i][j + 1].occupee != -1 || p->plateau[i - 1][j - 1].occupee != -1 ||
                             p->plateau[i - 1][j].occupee != -1 || p->plateau[i - 1][j + 1].occupee != -1
                             || p->plateau[i - 2][j - 3].occupee != -1 || p->plateau[i - 2][j - 2].occupee != -1 ||
                             p->plateau[i - 2][j - 1].occupee != -1 || p->plateau[i - 2][j].occupee != -1 ||
                             p->plateau[i - 2][j + 1].occupee != -1 || p->plateau[i - 2][j + 2].occupee != -1 ||
                             p->plateau[i - 1][j - 3].occupee != -1 || p->plateau[i - 1][j - 2].occupee != -1 ||
                             p->plateau[i - 1][j + 2].occupee != -1 || p->plateau[i][j - 3].occupee != -1 ||
                             p->plateau[i][j - 2].occupee != -1 || p->plateau[i][j + 2].occupee != -1 ||
                             p->plateau[i + 1][j - 3].occupee != -1 || p->plateau[i + 1][j - 2].occupee != -1 ||
                             p->plateau[i + 1][j + 2].occupee != -1))) {
                    p->erreur = true;

                    /// On ne fait rien. L'une des cases est occupée ou la maison se situe en dehors de la map.
                } else {
                    variableOnFaitLeBFS = true;
                    ajoutSommet(p, i, j);
                    /// selon le choix de la boite à outils on place le batiments concernés
                    switch (p->choix) {
                        case 1:
                            if (p->argentbanque >= 10) {
                                p->argentbanque = p->argentbanque - 10;
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son1 = NULL;
                                son1 = al_load_sample("../route2.wav");
                                al_play_sample(son1, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                                ajouterTableauCase(p, i, j, p->g->ordre);
                                verifierNumeroProximite(p, i, j);
                            }
                            break;
                        case 2 :
                            if (p->argentbanque >= 1000) {
                                p->argentbanque = p->argentbanque - 1000;
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son2 = NULL;
                                son2 = al_load_sample("../maison1.wav");
                                al_play_sample(son2, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
                                placementIndiceDeProximite(p, i, j);
                            }
                            break;
                        case 3 :
                            if (p->argentbanque >= 100000) {
                                p->argentbanque = p->argentbanque - 100000; // achat centrale
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son3 = NULL;

                                son3 = al_load_sample("../elec.wav");
                                al_play_sample(son3, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
                                placementIndiceDeProximite(p, i, j);
                            }
                            break;
                        case 4 :
                            if (p->argentbanque >= 100000) {
                                p->argentbanque = p->argentbanque - 100000;
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son4 = NULL;
                                son4 = al_load_sample("../eau1.wav");
                                al_play_sample(son4, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
                                placementIndiceDeProximite(p, i, j);
                            }
                            break;
                        case 5 :
                            if (p->argentbanque >= 100000) {
                                p->argentbanque = p->argentbanque - 100000;
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son5 = NULL;
                                son5 = al_load_sample("../pompier.wav");
                                al_play_sample(son5, 6, 0, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
                                placementIndiceDeProximite(p, i, j);
                            }
                            break;
                        case 6 :
                            al_reserve_samples(1);
                            ALLEGRO_SAMPLE* son6 = NULL;
                            son6=al_load_sample("../biblio.wav");
                            al_play_sample(son6, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        case 7 :
                            al_reserve_samples(1);
                            ALLEGRO_SAMPLE* son7 = al_load_sample("../ecole.wav");
                            al_play_sample(son7, 1, 0, 3, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        case 8 :
                            al_reserve_samples(1);
                            ALLEGRO_SAMPLE* son8 = al_load_sample("../park1.wav");
                            al_play_sample(son8, 1, 0, 3, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;

                    }
                    detectionContours(p, p->g->pSommet[p->g->ordre]);
                    switch (p->choix) {
                        case 1 : /// route
                            if (p->argentbanque >= 10) {
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son1 = al_load_sample("../route2.wav");
                                al_play_sample(son1, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                                p->plateau[i][j].batiment.typeBatiments = 1;
                                p->plateau[i][j].route = 1;
                                p->plateau[i][j].occupee = 1;
                            }
                            break;
                        case 2 :/// terrain vague
                            if (p->argentbanque >= 1000) {
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son2 = al_load_sample("../maison1.wav");
                                al_play_sample(son2, 1, 0, 3, ALLEGRO_PLAYMODE_ONCE, NULL);

                                /// p->plateay[i - 1][j].batiment.occupe = 2 ;
                                p->plateau[i][j].batiment.typeBatiments = 2;
                                p->plateau[i][j].occupee = 1;
                                p->plateau[i - 1][j].occupee = 1;
                                p->plateau[i + 1][j].occupee = 1;
                                p->plateau[i][j - 1].occupee = 1;
                                p->plateau[i][j + 1].occupee = 1;
                                p->plateau[i - 1][j - 1].occupee = 1;
                                p->plateau[i - 1][j + 1].occupee = 1;
                                p->plateau[i + 1][j - 1].occupee = 1;
                                p->plateau[i + 1][j + 1].occupee = 1;
                            }
                            break;
                        case 3 :/// centrale
                            if (p->argentbanque >= 100000) {
                                al_reserve_samples(1);
                                ALLEGRO_SAMPLE *son3 = NULL;
                                son3 = al_load_sample("../elec.wav");
                                al_play_sample(son3, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
                                p->plateau[i][j].batiment.typeBatiments = 3;
                                p->plateau[i][j].occupee = 1;
                                p->plateau[i - 1][j].occupee = 1;
                                p->plateau[i + 1][j].occupee = 1;
                                p->plateau[i][j - 1].occupee = 1;
                                p->plateau[i][j + 1].occupee = 1;
                                p->plateau[i - 1][j + 1].occupee = 1;
                                p->plateau[i + 1][j - 1].occupee = 1;
                                p->plateau[i - 1][j - 1].occupee = 1;
                                p->plateau[i + 1][j + 1].occupee = 1;//Carré
                                p->plateau[i][j + 2].occupee = 1;
                                p->plateau[i + 1][j + 2].occupee = 1;
                                p->plateau[i - 1][j + 2].occupee = 1;
                                p->plateau[i][j - 2].occupee = 1;
                                p->plateau[i + 1][j - 2].occupee = 1;
                                p->plateau[i - 1][j - 2].occupee = 1;
                                p->plateau[i][j - 3].occupee = 1;
                                p->plateau[i + 1][j - 3].occupee = 1;
                                p->plateau[i - 1][j - 3].occupee = 1;
                                p->plateau[i - 2][j - 3].occupee = 1;
                                p->plateau[i - 2][j - 2].occupee = 1;
                                p->plateau[i - 2][j - 1].occupee = 1;
                                p->plateau[i - 2][j].occupee = 1;
                                p->plateau[i - 2][j + 1].occupee = 1;
                                p->plateau[i - 2][j + 1].occupee = 1;
                                p->plateau[i - 2][j + 2].occupee = 1;
                            }
                            break;
                        case 4 : /// chateau d'eau
                            if (p->argentbanque >= 100000) {
                                p->plateau[i][j].batiment.typeBatiments = 4;
                                p->plateau[i][j].occupee = 1;
                                p->plateau[i - 1][j].occupee = 1;
                                p->plateau[i + 1][j].occupee = 1;
                                p->plateau[i][j - 1].occupee = 1;
                                p->plateau[i][j + 1].occupee = 1;
                                p->plateau[i - 1][j + 1].occupee = 1;
                                p->plateau[i + 1][j - 1].occupee = 1;
                                p->plateau[i - 1][j - 1].occupee = 1;
                                p->plateau[i + 1][j + 1].occupee = 1;//Carré
                                p->plateau[i][j + 2].occupee = 1;
                                p->plateau[i + 1][j + 2].occupee = 1;
                                p->plateau[i - 1][j + 2].occupee = 1;
                                p->plateau[i][j - 2].occupee = 1;
                                p->plateau[i + 1][j - 2].occupee = 1;
                                p->plateau[i - 1][j - 2].occupee = 1;
                                p->plateau[i][j - 3].occupee = 1;
                                p->plateau[i + 1][j - 3].occupee = 1;
                                p->plateau[i - 1][j - 3].occupee = 1;
                                p->plateau[i - 2][j - 3].occupee = 1;
                                p->plateau[i - 2][j - 2].occupee = 1;
                                p->plateau[i - 2][j - 1].occupee = 1;
                                p->plateau[i - 2][j].occupee = 1;
                                p->plateau[i - 2][j + 1].occupee = 1;
                                p->plateau[i - 2][j + 1].occupee = 1;
                                p->plateau[i - 2][j + 2].occupee = 1;
                            }
                            break;
                        case 5 :
                            if (p->argentbanque >= 100000) {
                                p->plateau[i][j].batiment.typeBatiments = 5;
                                p->plateau[i][j].occupee = 1;
                                p->plateau[i - 1][j].occupee = 1;
                                p->plateau[i + 1][j].occupee = 1;
                                p->plateau[i][j - 1].occupee = 1;
                                p->plateau[i][j + 1].occupee = 1;
                                p->plateau[i - 1][j + 1].occupee = 1;
                                p->plateau[i + 1][j - 1].occupee = 1;
                                p->plateau[i - 1][j - 1].occupee = 1;
                                p->plateau[i + 1][j + 1].occupee = 1;//Carré
                                p->plateau[i][j + 2].occupee = 1;
                                p->plateau[i + 1][j + 2].occupee = 1;
                                p->plateau[i - 1][j + 2].occupee = 1;
                                p->plateau[i][j - 2].occupee = 1;
                                p->plateau[i + 1][j - 2].occupee = 1;
                                p->plateau[i - 1][j - 2].occupee = 1;
                                p->plateau[i][j - 3].occupee = 1;
                                p->plateau[i + 1][j - 3].occupee = 1;
                                p->plateau[i - 1][j - 3].occupee = 1;
                                p->plateau[i - 2][j - 3].occupee = 1;
                                p->plateau[i - 2][j - 2].occupee = 1;
                                p->plateau[i - 2][j - 1].occupee = 1;
                                p->plateau[i - 2][j].occupee = 1;
                                p->plateau[i - 2][j + 1].occupee = 1;
                                p->plateau[i - 2][j + 1].occupee = 1;
                                p->plateau[i - 2][j + 2].occupee = 1;
                            }
                            break;
                        case 6 :

                            p->plateau[i][j].batiment.typeBatiments = 6;


                            p->plateau[i][j].occupee = 1;
                            p->plateau[i - 1][j].occupee = 1;
                            p->plateau[i + 1][j].occupee = 1;
                            p->plateau[i][j - 1].occupee = 1;
                            p->plateau[i][j + 1].occupee = 1;
                            p->plateau[i - 1][j - 1].occupee = 1;
                            p->plateau[i - 1][j + 1].occupee = 1;
                            p->plateau[i + 1][j - 1].occupee = 1;
                            p->plateau[i + 1][j + 1].occupee = 1;

                            break;
                        case 7 :
                            p->plateau[i][j].batiment.typeBatiments = 7;
                            p->plateau[i][j].occupee = 1;
                            p->plateau[i - 1][j].occupee = 1;
                            p->plateau[i + 1][j].occupee = 1;
                            p->plateau[i][j - 1].occupee = 1;
                            p->plateau[i][j + 1].occupee = 1;
                            p->plateau[i - 1][j + 1].occupee = 1;
                            p->plateau[i + 1][j - 1].occupee = 1;
                            p->plateau[i - 1][j - 1].occupee = 1;
                            p->plateau[i + 1][j + 1].occupee = 1;//Carré
                            p->plateau[i][j + 2].occupee = 1;
                            p->plateau[i + 1][j + 2].occupee = 1;
                            p->plateau[i - 1][j + 2].occupee = 1;
                            p->plateau[i][j - 2].occupee = 1;
                            p->plateau[i + 1][j - 2].occupee = 1;
                            p->plateau[i - 1][j - 2].occupee = 1;
                            p->plateau[i][j - 3].occupee = 1;
                            p->plateau[i + 1][j - 3].occupee = 1;
                            p->plateau[i - 1][j - 3].occupee = 1;
                            p->plateau[i - 2][j - 3].occupee = 1;
                            p->plateau[i - 2][j - 2].occupee = 1;
                            p->plateau[i - 2][j - 1].occupee = 1;
                            p->plateau[i - 2][j].occupee = 1;
                            p->plateau[i - 2][j + 1].occupee = 1;
                            p->plateau[i - 2][j + 1].occupee = 1;
                            p->plateau[i - 2][j + 2].occupee = 1;
                            break;
                        case 8 :
                            p->plateau[i][j].batiment.typeBatiments = 8;

                            p->plateau[i][j].occupee =1;
                            p->plateau[i-1][j].occupee =1;
                            p->plateau[i-1][j+1].occupee =1;
                            p->plateau[i][j+1].occupee =1;
                            break;

                    }
                }
            }
        }
    }

    if (p->choix == 4 && variableOnFaitLeBFS) { // Dans le cas ou on ajoute un nouveau chateau
        p->chateauEau[p->compteurChateau] = p->g->ordre;
        p->compteurChateau++;
        //remiseAZero(p);
        for (int i = 0; i < p->compteurChateau; i++) {
            bfs(p, p->chateauEau[i]);
        }

    } else if (p->choix == 3 && variableOnFaitLeBFS) { // dans le cas ou on ajoute une nouvelle centrale
        p->centraleElectrique[p->compteurCentrale] = p->g->ordre;
        p->compteurCentrale++;
        bfsElectricite(p, p->g->ordre);
    }
    if ((p->choix == 2 || p->choix == 1) && (p->compteurChateau != 0) &&
        (p->compteurCentrale != 0 && variableOnFaitLeBFS == true)) {
        remiseAZero(p);
        for (int i = 0; i < p->compteurChateau; i++) {
            //  printf("BFS sur le chateau %d\n", p->chateauEau[i]);
            bfs(p, p->chateauEau[i]);
        }
        for (int i = 0; i < p->compteurCentrale; i++) {
            // printf("BFS sur la centrale %d\n", p->chateauEau[i]);
            bfsElectricite(p, p->centraleElectrique[i]);
        }
    }
}

