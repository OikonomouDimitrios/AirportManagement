#ifndef __TSEvr__
#define __TSEvr__

#include <stdio.h>

typedef struct data {
    /*  members for airport data in array */
    unsigned int airport_id;
    char *name;
    char *city;
    char *country;
    char *iata;
    char *icao;
    int inbound;
    int outbound;
} TStoixeiouEvr;

int TSEvr_setValue(TStoixeiouEvr *target, TStoixeiouEvr source);

int TSEvr_readValue(const FILE *from, TStoixeiouEvr *Elem);

int TSEvr_writeValue(FILE *to, TStoixeiouEvr Elem);

#endif
