#ifndef FONCTION_H
#define FONCTION_H

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Station.h"
#include "ouvrir_csv.h"
#include "GeographicDistances.h"
#include "data.h"

#define DEBUG 0 // 0 ou 1

// Algo de Dijkstra, fonction principale
void algorithme(data_t* myData);

// Initialiser les différents tableaux
void init(data_t* myData);

// Avancer d'une station par rapport a la station actuelle
int avancerUneStation(data_t* myData, int indexStationSourceCourrante);

// Va comparer si deux Station (borne) sont les mêmes
bool isStationsDifferentes(data_t* myData, int indexA, int indexB);

void listeStation(Station** liste, int nbstation);

int choisirChemin(data_t* data, int type);

void choisirAutonomie(data_t* data);

void afficherURL(Station** listeToPrint, int nbStation);

#endif 
