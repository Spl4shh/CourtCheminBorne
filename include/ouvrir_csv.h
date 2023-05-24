#ifndef OUVRIR_CSV_H
#define OUVRIR_CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Station.h"
#include "data.h"

#define MAX_LINE_LENGTH 1000000
#define NB_MAX_STATION 100000

typedef struct Borne_t {
    char coordonnees[50];
    char puissance[50];
} Borne;

int lireFichier(const char* fichier, data_t* myData);

#endif
