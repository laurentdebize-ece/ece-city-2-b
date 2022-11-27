#include "include.h"



int main() {
    Principal p;
    p.leJeuEstLance = true;
    p.anciennepartiecharge = false;

    menuDebut(&p);
    if(!p.anciennepartiecharge) {
        initialisationstructure(&p);
        initialisationPlateau(&p);
    }
    if(p.anciennepartiecharge) {
        initialisationstructure(&p);
        initialisationPlateausauvegarder(&p ) ;
        p.anciennepartiecharge = false ;// on a fini de charger les données sauvegardées
    }

    if (p.leJeuEstLance == true){
        affichageMatrice(&p);
    }

    return 0;
}
