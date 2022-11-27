//
// Created by Louis Duvanel on 15/11/2022.
//

#include "include.h"

// On créé la base du graphe, ainsi que les données dependantes du graphe
Graphe *creerGraphe(Principal *p) {
    Graphe *Newgraphe = malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet *) malloc(300 * sizeof(pSommet));
    for (int i = 0; i < MAX_SOMMET; i++) {
        Newgraphe->pSommet[i] = (pSommet) malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->arete=NULL;
        Newgraphe->pSommet[i]->accesEauValide = false;
        Newgraphe->pSommet[i]->accesElecValide = false;
    }
    return Newgraphe;
}


// Fonction permettant la création d'une arete
void creerArete(Principal* p, int s1, int numeroSommetDeProximite){
    if(p->g->pSommet[numeroSommetDeProximite]->arete==NULL){

        Arete* Newarc=(pArete)malloc(sizeof(struct Arete));
        Newarc->numSommetNext=s1;
        Newarc->areteSuivante=NULL;
        p->g->pSommet[numeroSommetDeProximite]->arete=Newarc;

    } else {
        Arete* NewArc=p->g->pSommet[numeroSommetDeProximite]->arete;
        while(NewArc->areteSuivante != NULL) {
            NewArc=NewArc->areteSuivante;
        }
        ///On entre les valeurs pour la nouvelle arete
        NewArc->areteSuivante=(pArete)malloc(sizeof(struct Arete));
        NewArc->areteSuivante->numSommetNext=s1;
        NewArc->areteSuivante->areteSuivante=NULL;
        NewArc=NULL;
    }
}

// Fonction permettant l'ajout d'un sommet
void ajoutSommet(Principal *p, int i, int j) {
    p->g->ordre++;
    //Si c'est le premier sommet :
    if (p->g->pSommet == NULL) {
        // on donne son numéro, sa position et ses arete au sommet que l'on ajoute.
        p->g->pSommet[0]->numero = p->g->ordre;
        p->g->pSommet[0]->colonne = j;
        p->g->pSommet[0]->ligne = i;
        p->g->pSommet[0]->arete = NULL;
        switch (p->choix) {
            // On lie le type de batiment à son numéro de sommet
            case 1 :
                p->g->pSommet[0]->construction.typeBatiments = 1;
                break;
            case 2 :
                p->g->pSommet[0]->construction.typeBatiments = 2;
                break;
            case 3 :
                p->g->pSommet[0]->construction.typeBatiments = 3;
                break;
            case 4 :
                p->g->pSommet[0]->construction.typeBatiments = 4;
                break;
            case 5 :
                p->g->pSommet[0]->construction.typeBatiments = 5;
                break;
        }
    }
    // Sinon, le sommet prend la valeur de l'ordre du graphe (C'est la N-ième arète créé donc c'est le sommet N).
    else {
        p->g->pSommet[p->g->ordre]->numero = p->g->ordre;
        p->g->pSommet[p->g->ordre]->colonne = j;
        p->g->pSommet[p->g->ordre]->ligne = i;
        // On lie le type de batiment à son numéro de sommet
        switch (p->choix) {
            case 1 :
                p->g->pSommet[p->g->ordre]->construction.typeBatiments = 1;
                break;
            case 2 :
                p->g->pSommet[p->g->ordre]->construction.typeBatiments = 2;
                break;
            case 3 :
                p->g->pSommet[p->g->ordre]->construction.typeBatiments = 3;
                p->g->pSommet[p->g->ordre]->construction.capaciteElec=5000;
                break;
            case 4 :
                p->g->pSommet[p->g->ordre]->construction.typeBatiments = 4;
                p->g->pSommet[p->g->ordre]->construction.capaciteEau=2000; //Changer a 5000
                break;
            case 5 :
                p->g->pSommet[p->g->ordre]->construction.typeBatiments = 5;
                break;
        }
    }
}


// on vérifie la proximité entre 2 batiments, et si 2 batiments sont adjacents, on créé l'arete.
void verifierNumeroProximite(Principal* p, int i, int j){
    if(p->plateau[i-1][j].numeroSommetProximite!=-1){
        for(int k=0;k<4;k++) {
            if(p->plateau[i-1][j].adjacence[k]!=0) {
                creerArete(p, p->g->ordre, p->plateau[i-1][j].numeroSommetProximite);
                creerArete(p, p->plateau[i-1][j].numeroSommetProximite, p->g->ordre);
            }
        }
    }
    if(p->plateau[i][j+1].numeroSommetProximite!=-1){
        for(int k=0;k<4;k++) {
            if(p->plateau[i][j+1].adjacence[k]!=0) {
                creerArete(p, p->g->ordre, p->plateau[i][j+1].numeroSommetProximite);
                creerArete(p, p->plateau[i][j+1].numeroSommetProximite, p->g->ordre);
            }
        }
    }
    if(p->plateau[i+1][j].numeroSommetProximite!=-1){
        for(int k=0;k<4;k++) {
            if(p->plateau[i+1][j].adjacence[k]!=0) {
                creerArete(p, p->g->ordre, p->plateau[i+1][j].numeroSommetProximite);
                creerArete(p, p->plateau[i+1][j].numeroSommetProximite, p->g->ordre);
            }
        }
    }
    if(p->plateau[i][j-1].numeroSommetProximite!=-1){
        for(int k=0;k<4;k++) {
            if(p->plateau[i][j-1].adjacence[k]!=0) {
                creerArete(p, p->g->ordre, p->plateau[i][j-1].numeroSommetProximite);
                creerArete(p, p->plateau[i][j-1].numeroSommetProximite, p->g->ordre);
            }
        }
    }
}

// fonction détectant également l'adjacence entre 2 batiments
void detectionContours(Principal *p, pSommet s) {
    switch (s->construction.typeBatiments) {
        // selon le type de batiments les cases a vérifier sont différentes
        case 1 :
            if(p->plateau[s->ligne][s->colonne].adjacence != NULL){
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne][s->colonne].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne][s->colonne].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne][s->colonne].adjacence[i], p->g->ordre);
                    }
                }
            }
            break;

        case 2 :
            if (p->plateau[s->ligne][s->colonne - 1].adjacence != NULL){
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne][s->colonne-1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne][s->colonne-1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne][s->colonne-1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne][s->colonne+1].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne][s->colonne+1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne][s->colonne+1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne][s->colonne+1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne + 1][s->colonne].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne+1][s->colonne].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne+1][s->colonne].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne+1][s->colonne].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne -1][s->colonne].adjacence != NULL){
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne-1][s->colonne].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne-1][s->colonne].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne-1][s->colonne].adjacence[i], p->g->ordre);
                    }
                }

            }
            if (p->plateau[s->ligne - 1][s->colonne-1].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne-1][s->colonne-1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne-1][s->colonne-1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne-1][s->colonne-1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne - 1][s->colonne + 1].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne][s->colonne].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne-1][s->colonne+1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne-1][s->colonne+1].adjacence[i], p->g->ordre);
                    }
                }

            }
            if (p->plateau[s->ligne + 1][s->colonne + 1].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne][s->colonne].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne+1][s->colonne+1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne+1][s->colonne+1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne + 1][s->colonne - 1].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne+1][s->colonne-1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne+1][s->colonne-1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne+1][s->colonne-1].adjacence[i], p->g->ordre);
                    }
                }
            }
            break;

            case 3 :case 4:case 5:
            if (p->plateau[s->ligne][s->colonne + 2].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne][s->colonne + 2].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne][s->colonne + 2].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne][s->colonne + 2].adjacence[i], p->g->ordre);
                    }
                }

            }
            if (p->plateau[s->ligne-1][s->colonne + 2].adjacence !=NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne-1][s->colonne + 2].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne-1][s->colonne + 2].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne-1][s->colonne + 2].adjacence[i], p->g->ordre);
                    }
                }

            }
            if (p->plateau[s->ligne+1][s->colonne + 2].adjacence !=NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne-1][s->colonne + 2].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne-1][s->colonne + 2].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne-1][s->colonne + 2].adjacence[i], p->g->ordre);
                    }
                }

            }
            if (p->plateau[s->ligne -2][s->colonne + 2].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne -2][s->colonne + 2].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne -2][s->colonne + 2].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne -2][s->colonne + 2].adjacence[i], p->g->ordre);
                    }
                }
            }

            if (p->plateau[s->ligne - 2][s->colonne +1].adjacence !=NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne - 2][s->colonne +1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne - 2][s->colonne +1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne - 2][s->colonne +1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne - 2][s->colonne].adjacence != NULL) {
                for(int i=0;i<4;i++) {
                    if(p->plateau[s->ligne - 2][s->colonne].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne - 2][s->colonne].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne - 2][s->colonne].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne-2][s->colonne - 1].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne-2][s->colonne - 1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne-2][s->colonne - 1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne-2][s->colonne - 1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne -2][s->colonne - 2].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne -2][s->colonne - 2].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne -2][s->colonne - 2].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne -2][s->colonne - 2].adjacence[i], p->g->ordre);
                    }
                }

            }
            if (p->plateau[s->ligne -2][s->colonne-3].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne -2][s->colonne-3].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne -2][s->colonne-3].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne -2][s->colonne-3].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne - 1][s->colonne - 3].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne - 1][s->colonne - 3].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne - 1][s->colonne - 3].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne - 1][s->colonne - 3].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne][s->colonne - 3].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne][s->colonne - 3].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne][s->colonne - 3].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne][s->colonne - 3].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne +1][s->colonne - 3].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne +1][s->colonne - 3].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne +1][s->colonne - 3].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne +1][s->colonne - 3].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne +1][s->colonne-2].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne +1][s->colonne-2].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne +1][s->colonne-2].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne +1][s->colonne-2].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne +1][s->colonne -1].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne +1][s->colonne -1].adjacence[i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne +1][s->colonne -1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne +1][s->colonne -1].adjacence[i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne +1][s->colonne].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne +1][s->colonne].adjacence [i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne +1][s->colonne].adjacence [i]);
                        creerArete(p, p->plateau[s->ligne +1][s->colonne].adjacence [i], p->g->ordre);
                    }
                }
            }
            if (p->plateau[s->ligne +1][s->colonne+1].adjacence != NULL) {
                for(int i=0;i<4;i++){
                    if(p->plateau[s->ligne +1][s->colonne].adjacence [i]!=0) {
                        creerArete(p, p->g->ordre, p->plateau[s->ligne +1][s->colonne+1].adjacence[i]);
                        creerArete(p, p->plateau[s->ligne +1][s->colonne+1].adjacence[i], p->g->ordre);
                    }
                }
            }

            break;
    }
    affichageGraphe(p);
}


// Fonction permettant l'affichage du Graphe dans la console
void affichageGraphe(Principal* p){
    Arete* temp=NULL;
    if(p->g->ordre!=1) {
        for (int i = 1; i <= p->g->ordre; i++) {
            temp = p->g->pSommet[i]->arete;
            while (temp != NULL) {
                temp = temp->areteSuivante;
            }
            temp = NULL;
        }
        printf("\n\n");
    }
}

// Fonction permettant de rajouter un tableau sur les cases adjacentes à un batiment, ou bien
// rajouter le numéro du batiment dans un tableau s'il existe deja
void ajouterTableauCase(Principal* p, int i, int j, int numSommet){
   int k=0;
   /// s'il n'y a pas de tableau, on le créé et on ajoute le sommet au tableau :
   if( p->plateau[i + 1][j].adjacence==NULL) {
       p->plateau[i + 1][j].adjacence = calloc(4 , sizeof(int));
       p->plateau[i + 1][j].adjacence[0]=numSommet;
   }
   /// Sinon on ajoute juste le sommet :
   else{
       while(p->plateau[i + 1][j].adjacence[k]!=0){
           k++;
       }
       p->plateau[i + 1][j].adjacence[k]=numSommet;
       k=0;
   }
   /// s'il n'y a pas de tableau, on le créé et on ajoute le sommet au tableau :
   if(p->plateau[i - 1][j].adjacence==NULL) {
       p->plateau[i - 1][j].adjacence = calloc(4 , sizeof(int));
       p->plateau[i -1][j].adjacence[0]=numSommet;
   }/// Sinon on ajoute juste le sommet :
   else{
       while(p->plateau[i -1][j].adjacence[k]!=0){
           k++;
       }
       p->plateau[i - 1][j].adjacence[k]=numSommet;
       k=0;
   }
    /// s'il n'y a pas de tableau, on le créé et on ajoute le sommet au tableau :
   if(p->plateau[i][j + 1].adjacence==NULL) {
       p->plateau[i][j + 1].adjacence = calloc(4 , sizeof(int));
       p->plateau[i][j+1].adjacence[0]=numSommet;
   }/// Sinon on ajoute juste le sommet :
   else{
       while(p->plateau[i][j+1].adjacence[k]!=0){
           k++;
       }
       p->plateau[i][j+1].adjacence[k]=numSommet;
       k=0;
   }
    /// s'il n'y a pas de tableau, on le créé et on ajoute le sommet au tableau :
   if(p->plateau[i][j - 1].adjacence==NULL) {
       p->plateau[i][j - 1].adjacence = calloc(4 , sizeof(int));
       p->plateau[i][j-1].adjacence[0]=numSommet;
   }/// Sinon on ajoute juste le sommet :
   else {
       while(p->plateau[i][j-1].adjacence[k]!=0){
           k++;
       }
       p->plateau[i][j-1].adjacence[k]=numSommet;
   }
}