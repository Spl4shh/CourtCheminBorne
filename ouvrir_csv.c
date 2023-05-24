#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Station.h"
#include "ouvrir_csv.h"
#include "fonction.h"


int lireFichier(const char* fichier, data_t* myData) {
    FILE* file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier CSV\n");
        return -1;
    }

    char ligne[MAX_LINE_LENGTH];
    int nbStation = 0;
    bool entre_guillemets = false;

    // On ignore la première ligne qui contient les noms des champs
    fgets(ligne, MAX_LINE_LENGTH, file);

    while (fgets(ligne, MAX_LINE_LENGTH, file)) {
        
        Borne* borne = calloc(1, sizeof(Borne));
        Station* station = calloc(1, sizeof(Station));

        int longueur_ligne = strlen(ligne);
        int pos_champ = 0;
        int champ_actuel = 0;

        for (int i = 0; i < longueur_ligne; i++) {
            char c = ligne[i];

            if (c == '"') {
                entre_guillemets = !entre_guillemets;
            } else if (c == ',' && !entre_guillemets) {
                champ_actuel++;
                pos_champ = 0;
            } else {
                if (champ_actuel == 12) {
                    station->commune[pos_champ] = c;
                } else if (champ_actuel == 13) {
                    borne->coordonnees[pos_champ] = c;
                } else if (champ_actuel == 17) {
                    borne->puissance[pos_champ] = c;
                }
                pos_champ++;
            }
        }
        
        char* coordonnees = strtok(borne->coordonnees, ",");
        coordonnees = strtok(NULL, " "); // coordonnees contient maintenant la longitude avec un crochet en début de chaîne
        char* longitude = strtok(coordonnees, "[]");
        char* latitude = strtok(borne->coordonnees, "[] ");
        char* puissanceNominale = strtok(borne->puissance, ",");


        if (latitude != NULL && longitude != NULL && strlen(station->commune) > 4) {
            station->lat = strtod(latitude, NULL);
            station->lng = strtod(longitude, NULL);
            station->puissanceNominale = strtod(puissanceNominale, NULL);
            station->visite = false;

            if (nbStation < NB_MAX_STATION) {
                myData->listeStation[nbStation] = station;
                nbStation++;
                if(DEBUG){
                    printf("Station inséré \n");
                }
            }
            
            if(0){
                printf("Commune : %s, Latitude : %f, Longitude : %f, Puissance : %f\n", station->commune, station->lat, station->lng, station->puissanceNominale);
            }
        } else {
            if (DEBUG){
                printf("Erreur lors de la lecture des coordonnées\n");
            }
            free(station);
        }

        free(borne);
    }

    fclose(file);

    return nbStation;
}
