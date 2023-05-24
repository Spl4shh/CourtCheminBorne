#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Station.h"
#include "fonction.h"
#include "ouvrir_csv.h"
#include "data.h"
#include "GeographicDistances.h"

void algorithme(data_t* myData){
    if(DEBUG) {
        printf("[MAIN ALGO]: On entre dans l'algo principal \n");
        printf("[MAIN ALGO]: Nb de station = %d\n", myData->nbStation);
    }
    
    // Tableau qui va stocker l'ordre des stations visité 
    int nbStationParcours = 0;
    int* ordreParcours = malloc((myData->nbStation/2)*sizeof(int));
    ordreParcours[nbStationParcours] = myData->idStationSource;
    nbStationParcours++;
    myData->listeStation[myData->idStationSource]->visite = true;

    // Nombre de station visité (nombre de valeur dans ordreParcours)
    int indexNextStation = myData->idStationSource;

    if(DEBUG){
        printf("[MAIN ALGO]: Station de départ inséré dans la liste \n");
        
        Station* stationToPrint = myData->listeStation[ordreParcours[0]];
        printf("[MAIN ALGO]: Station de départ \n");
        printf("[MAIN ALGO]: Commune : %s, Latitude : %f, Longitude : %f, Puissance : %f\n", stationToPrint->commune, stationToPrint->lat, stationToPrint->lng, stationToPrint->puissanceNominale);

        Station* stationToPrint2 = myData->listeStation[myData->idStationDest];
        printf("[MAIN ALGO]: Station d'arrivé \n");
        printf("[MAIN ALGO]: Commune : %s, Latitude : %f, Longitude : %f, Puissance : %f\n", stationToPrint2->commune, stationToPrint2->lat, stationToPrint2->lng, stationToPrint2->puissanceNominale);

        printf("[MAIN ALGO]: Debut de la recherche \n");
    }

    while (isStationsDifferentes(myData, indexNextStation, myData->idStationDest))
    {
        indexNextStation = avancerUneStation(myData, indexNextStation);

        ordreParcours[nbStationParcours] = indexNextStation;
        nbStationParcours++;
        myData->listeStation[indexNextStation]->visite = true;

        if(DEBUG){
            Station* stationToPrint3 = myData->listeStation[ordreParcours[nbStationParcours-1]];
            printf("[MAIN ALGO RECHERCHE]: Station ajouté : \n");
            printf("[MAIN ALGO RECHERCHE]: Commune : %s, Latitude : %f, Longitude : %f, Puissance : %f\n", stationToPrint3->commune, stationToPrint3->lat, stationToPrint3->lng, stationToPrint3->puissanceNominale);
        }
    }

    if(DEBUG){
        printf("[MAIN ALGO]: On est arrivé \n");
        printf("[MAIN ALGO]: Affichage du parcours : \n");
        for (int i = 0; i < nbStationParcours; i++){
            Station* stationToPrint = myData->listeStation[ordreParcours[i]];
            printf("Commune : %s, Latitude : %f, Longitude : %f, Puissance : %f\n", stationToPrint->commune, stationToPrint->lat, stationToPrint->lng, stationToPrint->puissanceNominale);
        }
    }    

    Station* listeStationToPrint[nbStationParcours];

    for (int i = 0; i < nbStationParcours; i++) {
        Station* station = myData->listeStation[ordreParcours[i]];
        listeStationToPrint[i] = station;
    }
    
    printf("\nParcours : \n");
    listeStation(listeStationToPrint, nbStationParcours);

    afficherURL(listeStationToPrint, nbStationParcours);

    free(ordreParcours); 
}

void init(data_t* myData){
    myData->listeStation = malloc(NB_MAX_STATION*sizeof(Station));
    myData->idStationSource = 0;
    myData->idStationDest = 12785;

    myData->nbStation = lireFichier("./CSV/Donnees_voitures.csv", myData);

    choisirAutonomie(myData); // 100 km --> permet d'avoir plus de station, 500 permet d'avoir qqc de réaliste

    myData->idStationSource = choisirChemin(myData, 0);
    myData->idStationDest = choisirChemin(myData, 1);


    if (DEBUG){
        printf("Nb de station %d \n", myData->nbStation);
        printf("CP de la premiere station %s \n", myData->listeStation[0]->commune); 
    }
}

int avancerUneStation(data_t* myData, int indexStationSourceCourrante){
    // Parmis toutes les stations non visitées
    // Trouver celle qui se rapproche le plus de la station cible
    // Et dont la distance entre station courrante et station possible < autonomie
    // Cette etape rechargeras l'autonomie et donc on repartiras de la nouvelle station avec une autonomie max
    // retourneras l'index de la station à laquelle on avance
    
    int distanceNewStationToDest = INT_MAX;
    int indexNewStationCourrante = -1;

    Station* stationDest = myData->listeStation[myData->idStationDest];
    Station* stationSrcCourrante = myData->listeStation[indexStationSourceCourrante];

    for (int i = 0; i < myData->nbStation; i++){
        if (myData->listeStation[i]->visite != true) // = 0 donc on a pas encore visite
        {
            // assigne station a une struct
            Station* stationTest = myData->listeStation[i];

            // calculer distance entre station test et dest
            double distanceDest = geographicDistance(stationTest->lat, stationTest->lng, stationDest->lat, stationDest->lng);

            // calculer distance entre station test et sourceCourrante
            double distanceSource = geographicDistance(stationTest->lat, stationTest->lng, stationSrcCourrante->lat, stationSrcCourrante->lng);

            // Comparer les distances avec l'autonomie et chercher à réduire la distance avec la destination
            if(distanceSource < myData->autonomie && distanceDest < distanceNewStationToDest){
                distanceNewStationToDest = distanceDest;
                indexNewStationCourrante = i;
            }
        }
    }

    if(indexNewStationCourrante == -1) {
        printf("\nVotre autonomie n'est pas suffisante pour acceder à cette station.\n");
        exit(1);
    } 

    return indexNewStationCourrante;
}

bool isStationsDifferentes(data_t* myData, int indexA, int indexB){
    return (
        myData->listeStation[indexA]->lat != myData->listeStation[indexB]->lat &&
        myData->listeStation[indexA]->lng != myData->listeStation[indexB]->lng
    );
}

void listeStation(Station** liste, int nbstation){
    for (int i =0 ; i < nbstation; i++)
    {
        printf("ID : %d - Commune : %s - Puissance %5.2f - Latitude : %f - Longitude : %f \n", i , liste[i]->commune ,liste[i]->puissanceNominale, liste[i]->lat, liste[i]->lng);
    }
    
}

void choisirAutonomie(data_t* myData){
    int autonomie = 0;

    printf("Entrez l'autonomie de votre véhicule en kilomètre: \n");    
    scanf("%d", &autonomie);
    if(DEBUG){
        printf("L'autonomie choisie est de : %d \n", autonomie);
    }

    myData->autonomie = autonomie;
}

int choisirChemin(data_t* data,int type){
    int maxStation = 100;
    int compteur = 0;
    Station** liste_codeinsee = calloc(1,maxStation*sizeof(Station));
    int* liste_index = calloc(1,maxStation*sizeof(int));
    char code_insee[6];

    if(type == 0){
        printf("\nEntrez le code insee de la ville de départ (format nombre XXXXX)\n");
    } else if(type == 1) {
        printf("\nEntrez le code insee de la ville de d'arrivée (format nombre XXXXX)\n");
    }

    while (compteur == 0){
        scanf("%s", code_insee);
        if(DEBUG){
            printf("Le code insee choisi est : %s \n", code_insee);
        }

        for (int i = 0; i < data->nbStation; i++)
        {
            if(strcmp(code_insee, data->listeStation[i]->commune) == 0 && compteur < maxStation){
                bool stationExiste = false;

                for (int j = 0; j < compteur; j++)
                {
                    int indexA = liste_index[j];
                    int indexB = i;

                    if(!isStationsDifferentes(data, indexA, indexB)){
                        stationExiste = true;
                    }
                }
                
                if (!stationExiste) {
                    liste_index[compteur] = i;
                    liste_codeinsee[compteur] = data->listeStation[i];
                    compteur +=1;
                }
            }
        }

        if(compteur == 0){
            printf("Désolé, aucune station trouvée\n");
            printf("Veuillez saisir un autre code\n");
        }
    }
    
    
    int station_select = -1;
    if (compteur > 1) {
        listeStation(liste_codeinsee, compteur);

        while (station_select < 0 || station_select > compteur) {
            printf("Donnez l'ID de la station voulue : \n");
            scanf("%d",&station_select);
        }
        
        if(DEBUG){
            printf("id choisi global : %d \n",liste_index[station_select]);
        }
    } else {
        station_select = 0;
        
        printf("\nVoici la station choisie : \n");
        listeStation(liste_codeinsee, compteur);
    }

    int indexChoisi = liste_index[station_select];
    
    free(liste_index);
    free(liste_codeinsee);

    return indexChoisi;
}

void afficherURL(Station** listeToPrint, int nbStation){
    printf("\nURL pour le parcours Google Maps : \n");
    printf("https://www.google.com/maps/dir/");

    for (int i = 0; i < nbStation; i++){
        Station* station = listeToPrint[i];

        printf("%f,%f/", station->lng,station->lat);
    }
    
    printf("@%f,%f,8z\n", listeToPrint[0]->lng, listeToPrint[0]->lat);
}

// fonction principale
int main() {
    data_t* myData = malloc(sizeof(data_t));

    if(DEBUG){
        printf("Allocation réussie \n");
    }

    init(myData);

    if(DEBUG){
        printf("Initialisation réussie \n");
        printf("CP de la 1ere station apres init = %s \n", myData->listeStation[0]->commune);
    }

    algorithme(myData);

    for (int i = 0; i < myData->nbStation; i++){
        Station* stationToFree = myData->listeStation[i];
        free(stationToFree); 
    }
    
    free(myData->listeStation);
    free(myData);

    return 0;
}
