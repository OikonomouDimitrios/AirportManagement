#include <stdlib.h>
#include <string.h>
#include "TSEvr.h"


int TSEvr_setValuesFromFileParsing(TStoixeiouEvr *t, int index, char *value) {
    if (value == NULL || t == NULL) {
        return -1;
    }
    switch (index) {
        case 0:
            t->airport_id = atoi(value);
            break;
        case 1:
            t->name = strdup(value);
            break;
        case 2:
            t->city = strdup(value);
            break;
        case 3:
            t->country = strdup(value);
            break;
        case 4:
            t->iata = strdup(value);
            break;
        case 5:
            t->icao = strdup(value);
            break;
        default:
            break;
    }
    return 0;
}

int TSEvr_readValue(const FILE *from, TStoixeiouEvr *Elem) {}

int TSEvr_writeValue(FILE *to, TStoixeiouEvr Elem) {}

