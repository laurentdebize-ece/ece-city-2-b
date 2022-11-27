#include "include.h"

void creerFile(File *file, int num) {
    file->first = malloc(sizeof(Maillon));
    file->first->numSommet = num;
    file->first->next = NULL;
    file->last = file->first;

}

//On initialise les couleur des sommes du graphe a B (Blanc) et le tableau des precedents a -1
void initialisationCouleurSommet(Principal *p) {
    p->maisonProche = (int *) malloc((MAX_MAISON - 1) * sizeof(int));
    for (int i = 0; i <= p->g->ordre; i++) {
        p->g->pSommet[i]->couleur = 'B';
    }
    p->compteurMaison = 0;

}

void enfilerMarquer(File *file, int sommet, Graphe *g) {
    //Creation d un maillon avec les caracteristiques du sommet adjacents qui suivra le maillon precedent

    file->last->next = (Maillon *) malloc(sizeof(Maillon)); //Enfilage
    file->last->next->numSommet = sommet;
    file->last->next->next = NULL;
    file->last = file->last->next;

    g->pSommet[sommet]->couleur = 'G'; //Marquage

}

//Cette fonction cherche les sommets adjacents au sommet du debut de la file. Quand tous ses voisins sont decouverts
//on lui attribue la couleur N (Noire)

void defiler(Principal* p, File* file, int num){

    //Maillon Initial, le chateau d eau sur lequel on applique le bfs

    while(file->first!=NULL){//Si plus la capacite d alimenter

        //Pfree=file->first;

        Arete* arc = p->g->pSommet[file->first->numSommet]->arete; //Creation d'un pointeur qui inspecte le premier de la file

        while (arc != NULL){ // Tant qu'il y'a un arc ou que les habitants n'ont pas tous de l'eau


            if(p->g->pSommet[arc->numSommetNext]->couleur=='B' && p->g->pSommet[arc->numSommetNext]->construction.typeBatiments==2){
                p->g->pSommet[arc->numSommetNext]->accesEauValide = false;

                p->maisonProche[p->compteurMaison]=arc->numSommetNext;//On inscrit la maison dans un tableau de maison proche qui nous servira pour bien repartir l'eau

                p->compteurMaison++;//On incremente un au compteur pour le tableau de maison proche

                enfilerMarquer(file, arc->numSommetNext, p->g);

                p->g->pSommet[arc->numSommetNext]->couleur='G';//NumSommetNext désigne le numero du sommet adjacents
                //Si on alimente une maison partiellement alimenté

                if(p->g->pSommet[arc->numSommetNext]->habitantSansEau>0){
                    if(p->g->pSommet[arc->numSommetNext]->habitantSansEau > p->g->pSommet[num]->construction.capaciteEau){
                        p->g->pSommet[arc->numSommetNext]->habitantSansEau-=p->g->pSommet[num]->construction.capaciteEau;
                        p->g->pSommet[arc->numSommetNext]->accesEauValide=false;
                    }
                    else{
                        p->g->pSommet[num]->construction.capaciteEau-=p->g->pSommet[arc->numSommetNext]->habitantSansEau;
                        p->g->pSommet[arc->numSommetNext]->accesEauValide=true;
                    }
                }
                //Si il y'a assez d'eau, on peut alimenter tous les habitants
                if(p->g->pSommet[num]->construction.capaciteEau - p->g->pSommet[arc->numSommetNext]->nombreHabitant >= 0){
                    //Habitation totalement alimentee
                    p->plateau[p->g->pSommet[arc->numSommetNext]->ligne][p->g->pSommet[arc->numSommetNext]->colonne].batiment.numeroChateauAssociee = num;
                    p->g->pSommet[num]->construction.capaciteEau -= p->g->pSommet[arc->numSommetNext]->nombreHabitant;
                    p->g->pSommet[arc->numSommetNext]->accesEauValide=true;

                }
                if((p->g->pSommet[num]->construction.capaciteEau - p->g->pSommet[arc->numSommetNext]->nombreHabitant) < 0){
                    //Habitation partiellement alimentee
                    p->g->pSommet[arc->numSommetNext]->habitantSansEau = p->g->pSommet[arc->numSommetNext]->nombreHabitant - p->g->pSommet[num]->construction.capaciteEau;

                }
            }

            if (p->g->pSommet[arc->numSommetNext]->couleur == 'B' && p->g->pSommet[arc->numSommetNext]->construction.typeBatiments==1){
                //Dans le cas d'une route a proximite
                enfilerMarquer(file, arc->numSommetNext, p->g);
            }

            arc = arc->areteSuivante;
        }
        p->g->pSommet[file->first->numSommet]->couleur = 'N';
        file->first = file->first->next;
    }
}



void defilerElectricite(Principal* p, File* file, int num){
    //Maillon* Pfree=NULL;

    //Maillon Initial, la centrale sur laquel on applique le bfs



    while(file->first!=NULL){//tant qu'il existe toujours des maisons reliées et pas encore découvertes

        //Pfree=file->first;

        Arete* arc = p->g->pSommet[file->first->numSommet]->arete; //Creation d'un pointeur qui inspecte le premier de la file
        while (arc != NULL){ // Tant qu'il y'a un arc ou que les habitants n'ont pas tous de l'electricité

            if(p->g->pSommet[arc->numSommetNext]->couleur=='B' && p->g->pSommet[arc->numSommetNext]->construction.typeBatiments==2){
                p->g->pSommet[arc->numSommetNext]->accesElecValide = false;
                p->maisonProche[p->compteurMaison]=arc->numSommetNext;//On inscrit la maison dans un tableau de maison proche qui nous servira pour bien repartir l'eau
                p->compteurMaison++;//On incremente un au compteur pour le tableau de maison proche

                enfilerMarquer(file, arc->numSommetNext, p->g);

                p->g->pSommet[arc->numSommetNext]->couleur='G';//NumSommetNext désigne le numero du sommet adjacents

                //Si il y'a assez d'eau, on peut alimenter tous les habitants
                if(p->g->pSommet[num]->construction.capaciteElec - p->g->pSommet[arc->numSommetNext]->nombreHabitant >= 0){
                    //Habitation totalement alimentee
                    p->plateau[p->g->pSommet[arc->numSommetNext]->ligne][p->g->pSommet[arc->numSommetNext]->colonne].batiment.numeroCentraleAssociee = num;
                    p->g->pSommet[num]->construction.capaciteElec -= p->g->pSommet[arc->numSommetNext]->nombreHabitant;

                    p->g->pSommet[arc->numSommetNext]->accesElecValide=true;
                }
            }

            else if (p->g->pSommet[arc->numSommetNext]->couleur == 'B' && p->g->pSommet[arc->numSommetNext]->construction.typeBatiments==1){
                //Dans le cas d'une route a proximite
                enfilerMarquer(file, arc->numSommetNext, p->g);
                p->g->pSommet[arc->numSommetNext]->couleur='G';
            }

            arc = arc->areteSuivante;
            //affichageFile(file);
        }
        p->g->pSommet[file->first->numSommet]->couleur = 'N';
        file->first = file->first->next;
        //free(Pfree);
    }
}


void bfsElectricite(Principal *p, int num) {
    if (p->g->pSommet[num]->construction.typeBatiments == 3) {
        File file;
        creerFile(&file, num);
        initialisationCouleurSommet(p);
        p->g->pSommet[num]->couleur = 'G';
        p->g->pSommet[num]->construction.capaciteElec = 5000;
        defilerElectricite(p, &file, num);
    }
}

//On affiche les sommets selon le tableau des predecesseurs de maniere recursive


void bfs(Principal *p, int num) {
    if (p->g->pSommet[num]->construction.typeBatiments == 4) {
        File file;
        creerFile(&file, num);
        initialisationCouleurSommet(p);
        p->g->pSommet[num]->couleur = 'G';
        p->g->pSommet[num]->construction.capaciteEau = 5000;
        defiler(p, &file, num);

    }
}

/// remet à 0 les anciennes mesures de distances
void remiseAZero(Principal *p) {
    for (int i = 0; i < p->compteurMaison; i++) {
        p->maisonProche[i] = -1;
    }
    p->compteurMaison = 0;
    for (int j = 0; j < p->g->ordre; j++) {
        if (p->g->pSommet[j]->construction.typeBatiments == 2) {
            p->g->pSommet[j]->habitantSansEau = 0;
            p->g->pSommet[j]->accesEauValide = false;
            p->g->pSommet[j]->couleur = 'B';
        }
    }

}
