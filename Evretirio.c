#include <assert.h>
#include "Evretirio.h"

#include "RedBlackTreeFunctions.h" /* h Ylopoihsh sas toy R/B */

#define MAXSIZE 7200
struct EvrNode {
    TStoixeiouEvr *DataArray; /* array of size MaxSize */
    int Index;              /* index of first available element in array */
    RedBlackTree redBlackTree;     /* Root of DDA */
} EvrNode;


EvrPtr EvrConstruct(int MaxSize) {
    EvrPtr evrPtr = malloc(sizeof(struct EvrNode));
    evrPtr->DataArray = malloc(MaxSize * sizeof(TStoixeiouDDA));
    assert(evrPtr->DataArray != NULL);
    evrPtr->Index = 0;
    evrPtr->redBlackTree = rbt_initialize_tree(
            (int (*)(const void *, const void *)) TSDDA_compare,
            (void (*)(const void *, char *, size_t)) integer_type_to_string);

    return evrPtr;

}

int Evr_Insert(EvrPtr E, TStoixeiouEvr Data) {
    // Get the next available index in DataArray
    int index = E->Index;
    if (index >= MAXSIZE) {
        return -1; // Error: DataArray is full
    }
    // Insert the data into the DataArray at the next available index
    E->DataArray[index] = Data;
    E->Index++;
    TStoixeiouDDA *tStoixeiouDda = malloc(sizeof(TStoixeiouDDA));
tStoixeiouDda->key
    // Add the data to the red-black tree
    int error_rbt = 0;
    rbt_insert_node(&(E->redBlackTree), &E->DataArray[index], &error_rbt);
    if (error_rbt) {
        printf("failed to insert into RBT");
        return -1;
    }

    return 0;
}

int EvrSearch(EvrPtr E, keyType key, int InOut, int *found) {}


int Evr_PrintAll(EvrPtr E, FILE *out, int *counter) {

}

int Evr_katastrofi(EvrPtr *E) {}
