#ifndef STATION_H
#define STATION_H
#include <stdbool.h>


typedef struct Station_t {
    bool visite;
    double lat; 
    double lng;
    double puissanceNominale;
    char commune[50];
    //int nbStation;
} Station;

#endif
