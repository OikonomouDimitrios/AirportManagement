#include <assert.h>
#include "Evretirio.h"

#include "RedBlackTreeFunctions.h" /* h Ylopoihsh sas toy R/B */

struct EvrNode {
    TStoixeiouEvr *DataArray; /* array of size MaxSize */
    int Index;              /* index of first available element in array */
    RedBlackTree redBlackTree;     /* Root of DDA */
    AirportStructPtr airportStructPtr;
} EvrNode;

struct AirportStruct {
    unsigned int airport_id;
    unsigned int array_position;
} AirportStruct;

typedef struct Airport {
    unsigned int airport_id;
    char name[256];
    char city[256];
    char country[256];
    char iata[4];
    char icao[5];
} Airport;

EvrPtr EvrConstruct(int MaxSize) {
    EvrPtr evrPtr = malloc(sizeof(struct EvrNode));
    evrPtr->DataArray = malloc(MaxSize * sizeof(Airport));
    assert(evrPtr->DataArray != NULL);
    evrPtr->Index = 0;
    evrPtr->redBlackTree = rbt_initialize_tree();
    evrPtr->airportStructPtr = malloc(MaxSize * sizeof(struct AirportStruct));
    for (int i = 0; i < MaxSize; i++) {
        evrPtr->airportStructPtr[i].airport_id = 0;
        evrPtr->airportStructPtr[i].array_position = i;
    }
    return evrPtr;

}

int Evr_Insert(EvrPtr E, TStoixeiouEvr Data) {}

int EvrSearch(EvrPtr E, keyType key, int InOut, int *found) {}

int AirportStructCompare(const AirportStructPtr a, const AirportStructPtr b) {
    if (a->airport_id < b->airport_id) {
        return -1;
    } else if (a->airport_id > b->airport_id) {
        return 1;
    } else {
        return 0;
    }
}

int Evr_PrintAll(EvrPtr E, FILE *out, int *counter) {

}

int Evr_katastrofi(EvrPtr *E) {}
