#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "GeographicDistances.h"
#define M_PI       3.14159265358979323846

double rad(double degre) {
    return degre * ( M_PI / 180.0 );  
}

double geographicDistance(double lat1,double lng1, double lat2, double lng2){
    //Rayon moyen de la terre : 6 371km selon l'Union géodésique et géophysique internationale (UGGI), CF : https://fr.wikipedia.org/wiki/Rayon_de_la_Terre
    return acos(sin(rad(lat1))*sin(rad(lat2))+cos(rad(lat1))*cos(rad(lat2))*cos(rad(lng2)-rad(lng1))) * 6371;
}
