

#include "include.h"

bool quitter(Principal *p) {
    bool fin = false;
    p->quitter = al_load_bitmap("../images/quitter_sauvegarde.png");
    al_draw_bitmap(p->quitter, 0, 0, 0);
    if (p->sauvegarder) {
        p->fichiersauvegarde = fopen("../fichier_sauvegarde", "w");
        p->informations = fopen("../informations_sauvegarde", "w");
        for (int i = 0; i < LIGNE; i++) {
            for (int j = 0; j < COLONNE; j++) {
                if (p->plateau[i][j].batiment.typeBatiments != 0) {
                    fprintf(p->fichiersauvegarde, " %d", p->plateau[i][j].batiment.typeBatiments);
                } else if (p->plateau[i][j].occupee != -1) {
                    fprintf(p->fichiersauvegarde, " 9"); // 9 pour une case occupée
                } else {
                    fprintf(p->fichiersauvegarde, " 0");
                }
                fprintf(p->informations, " %d", p->plateau[i][j].batiment.stadeDeConstruction);
            }
            // on passe à la prochaine ligne
            fprintf(p->informations, "\n");
            fprintf(p->fichiersauvegarde, "\n");
        }
        // on récupère le reste des informations concernant le graphe
        fprintf(p->informations, " %d", p->nbrhabitants);
        fprintf(p->fichiersauvegarde, "\n");
        fprintf(p->informations, " %d", p->argentbanque);
        fclose(p->fichiersauvegarde);
        fclose(p->informations);
        fin = true;
    }
    return fin;
}

void initialisationPlateausauvegarder(Principal *p) { // Commence à x=10 & y=10
    int x = 10;
    int y = 10;
    int occupe = 0;
    int stade  = 0 ;
    p->fichiersauvegarde = fopen("../fichier_sauvegarde", "r");
    p->informations = fopen("../informations_sauvegarde" , "r");
    if (p->fichiersauvegarde == NULL) {
        printf("Erreur d'ouverture\n");
    }
    for (int i = 0; i < LIGNE; i++) {
        for (int j = 0; j < COLONNE; j++) {
            /// on récupère les infos des 2 fichiers textes
            /// informations : stade des batiments + nbr habitants + argent
            /// fichier sauvegarde : les types de batiments
            fscanf(p->informations , " %d" , &stade) ;
            fscanf(p->fichiersauvegarde, " %d", &occupe);
            if (occupe != 0) {
                p->plateau[i][j].occupee = 1;
                p->plateau[i][j].batiment.typeBatiments = occupe;
                if (occupe == 2) {
                    // on récupère le stade de construction de la construction
                    p->plateau[i][j].batiment.stadeDeConstruction = stade;
                }
            } else if (occupe == 0) {
                // case non occupée
                p->plateau[i][j].occupee = -1;
                p->plateau[i][j].batiment.typeBatiments = 0;
            }
            p->plateau[i][j].positionX = x;
            p->plateau[i][j].positionY = y;
            p->plateau[i][j].numeroSommetProximite = -1;
            x += 20;
            occupe = 0;
        }
        printf("\n");
        x = 10;
        y += 20;
    }
    fscanf(p->informations , " %d" , &stade);
    p->nbrhabitants = stade;
    fscanf(p->informations , " %d" , &stade);
    p->argentbanque = stade;
    fclose(p->fichiersauvegarde);
    fclose(p->informations);
}



