#ifndef __TSEvr__
#define __TSEvr__

#include <stdio.h>

typedef struct data {
    /*  members for airport data in array */
    unsigned int airport_id;
    char name[256];
    char city[256];
    char country[256];
    char iata[4];
    char icao[5];
} TStoixeiouEvr;

int TSEvr_setValue(TStoixeiouEvr *target, TStoixeiouEvr source);

int TSEvr_readValue(FILE *from, TStoixeiouEvr *Elem);

int TSEvr_writeValue(FILE *to, TStoixeiouEvr Elem);

#endif
