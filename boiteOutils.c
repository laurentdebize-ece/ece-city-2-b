//
// Created by Louis Duvanel on 03/11/2022.
//

#include "include.h"


void initialisationImageBoiteOutils(Principal* p){
    p->affichageBoiteOutils[0]= al_load_bitmap("../images/route.png");//Route
    p->affichageBoiteOutils[1]= al_load_bitmap("../images/terrain.png");//Terrain vague
    p->affichageBoiteOutils[2]= al_load_bitmap("../images/usine.png");//Usine électrique
    p->affichageBoiteOutils[3]= al_load_bitmap("../images/chateauEau.png");//Chateau d'eau
    p->affichageBoiteOutils[4]= al_load_bitmap("../images/pompier.png");//Caserne
    p->affichageBoiteOutils[5]= al_load_bitmap("../images/menu-boite-outils.png");//Fond principal de la boite O
    p->affichageBoiteOutils[7]= al_load_bitmap("../images/arrow.png");//
    p->affichageBoiteOutils[8]= al_load_bitmap("../images/arrow2.png");
    p->affichageBoiteOutils[9]= al_load_bitmap("../images/outils.png");
    p->affichageBoiteOutils[10] = al_load_bitmap("../images/library.png");
    p->affichageBoiteOutils[11] = al_load_bitmap("../images/eceBatiment.png");
    p->affichageBoiteOutils[12] = al_load_bitmap("../images/parc.png");

    p->affichageTexteBoiteOutils= al_load_font("../JosefinSans-Italic-VariableFont_wght.ttf", 20, 0);
}

void menuBoiteOutils(Principal* p){
    al_draw_bitmap(p->affichageBoiteOutils[5], OUTILS_PLATEAU_X, OUTILS_PLATEAU_Y,0);
    al_draw_filled_rectangle(RECT_TRANSI_X, RECT_TRANSI_Y, RECT_TRANSI_X2,RECT_TRANSI_Y2, al_map_rgb(60,80,177));
    al_draw_bitmap(p->affichageBoiteOutils[7],RECT_TRANSI_X+10, RECT_TRANSI_Y,0);

    int decalage=50;

    for(int i=0;i<3;i++){
        switch(i){
            case 0 :
                al_draw_bitmap(p->affichageBoiteOutils[0], rectX1+decalage+10, rectY1+175, 0);
                break;
            case 1 :
                al_draw_bitmap(p->affichageBoiteOutils[1], rectX1+decalage+10, rectY1+175, 0);
                break;
            case 2 :
                al_draw_bitmap(p->affichageBoiteOutils[2], rectX1+decalage+10, rectY1+175, 0);
                break;
        }
        decalage+=280;
    }
}

void menuBoiteOutils3(Principal* p){
    al_draw_bitmap(p->affichageBoiteOutils[5], OUTILS_PLATEAU_X, OUTILS_PLATEAU_Y,0);
    al_draw_filled_rectangle(RECT_TRANSI_2_X, RECT_TRANSI_2_Y, RECT_TRANSI_2_X2,RECT_TRANSI_2_Y2, al_map_rgb(60,80,177));
    al_draw_bitmap(p->affichageBoiteOutils[8],RECT_TRANSI_2_X+10, RECT_TRANSI_2_Y,0);


    int decalage=70;
    for(int i=0;i<3;i++){
        switch(i){
            case 0 :
                al_draw_bitmap(p->affichageBoiteOutils[10], rectX1+decalage+10, rectY1+175, 0);
                break;
            case 1 :
                al_draw_bitmap(p->affichageBoiteOutils[11], rectX1+decalage+10, rectY1+175, 0);
                break;
            case 2 :
                al_draw_bitmap(p->affichageBoiteOutils[12], rectX1+decalage+10, rectY1+175, 0);
                break;
        }
        decalage+=280;
    }
}

void menuBoiteOutils2(Principal* p){
    al_draw_bitmap(p->affichageBoiteOutils[5], OUTILS_PLATEAU_X, OUTILS_PLATEAU_Y,0);
    al_draw_filled_rectangle(RECT_TRANSI_X, RECT_TRANSI_Y, RECT_TRANSI_X2,RECT_TRANSI_Y2, al_map_rgb(60,80,177));
    al_draw_filled_rectangle(RECT_TRANSI_2_X, RECT_TRANSI_2_Y, RECT_TRANSI_2_X2,RECT_TRANSI_2_Y2, al_map_rgb(60,80,177));
    al_draw_bitmap(p->affichageBoiteOutils[7],RECT_TRANSI_X+10, RECT_TRANSI_Y,0);
    al_draw_bitmap(p->affichageBoiteOutils[8],RECT_TRANSI_2_X+10, RECT_TRANSI_2_Y,0);

    ///NIVEAU _1
    al_draw_filled_rectangle(700, 300, 850, 370, al_map_rgb(50, 50, 200));
    al_draw_textf(p->police2, al_map_rgb(255, 255, 255), 730, 320, 0,
                  " Niveau -1");
    /// NIVEAU -2
    al_draw_filled_rectangle(700, 400, 850, 470, al_map_rgb(200, 200, 50));
    al_draw_textf(p->police2, al_map_rgb(255, 255, 255), 730, 420, 0,
                  "Niveau -2");

    int decalage=90;

    for(int i=0;i<2;i++){
        switch(i){
            case 0 :
                al_draw_bitmap(p->affichageBoiteOutils[3], rectX1+decalage+10, rectY1+175, 0);
                break;
            case 1 :
                al_draw_bitmap(p->affichageBoiteOutils[4], rectX1+decalage+30, rectY1+175, 0);
                break;
        }
        decalage+=280;
    }
}

void reperageCliqueCaseOutils(Principal* p, int x, int y, bool* boiteOutils){
    if(p->secondePageOutils==true && 700<=x && x<=850 && 300<=y && y<=370){
        p->niveau1=true;
        p->secondePageOutils=false;
        *boiteOutils=false;
    }
    if(p->secondePageOutils==true && 700<=x && x<=850 && 400<=y && y<=470){
        p->niveau2=true;
        p->secondePageOutils=false;
        *boiteOutils=false;
    }
    if(OUTILS_PLATEAU_X+90<=x &&x<=OUTILS_PLATEAU_X+90+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=1;

        p->enCoursdeConstruction=true;

        /*if(p->argentbanque >= 10){
            p->argentbanque =  p->argentbanque - 10 ; // achat route
        }*/
    }
    if(OUTILS_PLATEAU_X+370<=x &&x<=OUTILS_PLATEAU_X+370+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=2;
        p->enCoursdeConstruction=true;
        /*if(p->argentbanque >= 1000){
            p->argentbanque =  p->argentbanque - 1000 ; // achat terrain vague
        }*/
    }
    if(OUTILS_PLATEAU_X+650<=x &&x<=OUTILS_PLATEAU_X+650+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=3;
        p->enCoursdeConstruction=true;
        /*if(p->argentbanque >= 100000){
            p->argentbanque =  p->argentbanque - 100000 ; // achat centrale
        }*/
    }
    if( p->secondePageOutils==true && OUTILS_PLATEAU_X+90<=x &&x<=OUTILS_PLATEAU_X+90+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=4;
        p->enCoursdeConstruction=true;
        /*if(p->argentbanque >= 100000){
            p->argentbanque =  p->argentbanque - 100000 ; // achat chateau d'eau
        }*/
    }
    if(p->secondePageOutils==true && OUTILS_PLATEAU_X+370<=x &&x<=OUTILS_PLATEAU_X+370+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=5;
        p->enCoursdeConstruction=true;
        /*if(p->argentbanque >= 100000){
            p->argentbanque =  p->argentbanque - 100000 ; // achat caserne
        }*/
    }
    if(p->troisiemePageOutils==true && OUTILS_PLATEAU_X+90<=x &&x<=OUTILS_PLATEAU_X+90+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=6;
        p->enCoursdeConstruction=true;
        if(p->argentbanque >= 40000){
            p->argentbanque = p->argentbanque - 40000; //achat bibliotheque
            printf("Vous avez acheté une bibliotheque\n");
        }
    }
    if(p->troisiemePageOutils==true && OUTILS_PLATEAU_X+370<=x &&x<=OUTILS_PLATEAU_X+370+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=7;
        p->enCoursdeConstruction=true;
        if(p->argentbanque >= 50000){
            p->argentbanque = p->argentbanque - 50000; //achat ECE campus Lyon
            printf("Vous avez le campus de LYON\n");
        }
    }
    if(p->troisiemePageOutils==true && OUTILS_PLATEAU_X+650<=x &&x<=OUTILS_PLATEAU_X+650+TAILLE_AFFICHAGE_BOIS_X && CASE_OUTILS_Y<=y && y<=CASE_OUTILS_Y+TAILLE_AFFICHAGE_BOIS_Y){
        p->choix=8;
        p->enCoursdeConstruction=true;
        if(p->argentbanque >= 20000){
            p->argentbanque = p->argentbanque - 20000; //achat petit parc
            printf("Vous avez un petit parc\n");
        }
    }
}

