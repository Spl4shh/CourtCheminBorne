#ifndef DATA_H
#define DATA_H
#include "Station.h"

typedef struct data {
    int nbStation;
    Station** listeStation;
    int idStationSource;
    int idStationDest;
    int autonomie;
} data_t;

#endif
