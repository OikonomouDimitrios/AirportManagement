#include <assert.h>
#include "Evretirio.h"

#include "RedBlackTreeFunctions.h" /* h Ylopoihsh sas toy R/B */

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

int EvrInsert(EvrPtr E, TStoixeiouEvr Data) {
    // Get the next available index in DataArray
    int index = E->Index;
    if (index >= MAXSIZE) {
        return -1; // Error: DataArray is full
    }
    // Insert the data into the DataArray at the next available index
    E->DataArray[index] = Data;
    E->Index++;
    TStoixeiouDDA *tStoixeiouDda = malloc(sizeof(TStoixeiouDDA));
    TSDDA_setKey(&(tStoixeiouDda->key), Data.airport_id);
    TSDDA_setIndex(&(tStoixeiouDda->arrayIndex), index);
    // Add the data to the red-black tree
    int error_rbt = 0;
    rbt_insert_node(&(E->redBlackTree), tStoixeiouDda, &error_rbt);
    if (error_rbt) {
        printf("failed to insert into RBT");
        return -1;
    }

    return 0;
}

int EvrSearch(EvrPtr E, keyType key, int InOut, int *found) {}


int Evr_PrintAll(EvrPtr E, FILE *out, int *counter) {
    rbt_print_tree(&(E->redBlackTree), counter);
}

int Evr_katastrofi(EvrPtr *E) {
    rbt_free(&((*E)->redBlackTree));
    for (int i = 0; i < (*E)->Index; i++) {
        free((*E)->DataArray[i].name);
        free((*E)->DataArray[i].city);
        free((*E)->DataArray[i].country);
        free((*E)->DataArray[i].iata);
        free((*E)->DataArray[i].icao);
    }
    free((*E)->DataArray);
    free(E);
}

int Evr_ReadFromFile(EvrPtr E) {
    FILE *fp = fopen("../data/airportsWinRandom.txt", "r");
    char line[MAX_LINE_LENGTH];
    const char delimiter[] = ";\r";

    if (fp == NULL) {
        printf("Error: could not open file\n");
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        char *token;
        TStoixeiouEvr *tStoixeiouEvr = malloc(sizeof(TStoixeiouEvr));

        token = strtok(line, delimiter);
//        printf("Field 0: %s\n", token);
        tStoixeiouEvr->airport_id = atoi(token);


        token = strtok(NULL, delimiter);
        tStoixeiouEvr->name = malloc(strlen(token) + 1);
        strcpy(tStoixeiouEvr->name, token);
//        printf("Field 1: %s\n", token);

        token = strtok(NULL, delimiter);
        tStoixeiouEvr->city = malloc(strlen(token) + 1);
        strcpy(tStoixeiouEvr->city, token);
//        printf("Field 2: %s\n", token);

        token = strtok(NULL, delimiter);
        tStoixeiouEvr->country = malloc(strlen(token) + 1);
        strcpy(tStoixeiouEvr->country, token);
//        printf("Field 3: %s\n", token);

        token = strtok(NULL, delimiter);
        tStoixeiouEvr->iata = malloc(strlen(token) + 1);
        strcpy(tStoixeiouEvr->iata, token);
//        printf("Field 4: %s\n", token);

        token = strtok(NULL, delimiter);
        tStoixeiouEvr->icao = malloc(strlen(token) + 1);
        strcpy(tStoixeiouEvr->icao, token);
//        printf("Field 5: %s\n", token);

        EvrInsert(E, *tStoixeiouEvr);
    }
    fclose(fp);
    char file_name[] = "../data/routesWin.txt";
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", file_name);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *sourceAirport;
        char *destinationAirport;
        sourceAirport = strtok(line, delimiter);
        for (int i = 1; i < 4 && sourceAirport != NULL; i++) {
            sourceAirport = strtok(NULL, delimiter);
        }
        for (int i = 1; i < 3 && sourceAirport != NULL; i++) {
            destinationAirport = strtok(NULL, delimiter);
        }
        getchar();
        if (sourceAirport != NULL) {
            TStoixeiouDDA *tStoixeiouDda1 = (TStoixeiouDDA *) malloc(sizeof(TStoixeiouDDA));
            TSDDA_setKey(&(tStoixeiouDda1->key), atoi(sourceAirport));
            printf("The 4th value is: %s\n", sourceAirport);
            TStoixeiouDDA *tStoixeiouDda = rbt_find_node(E->redBlackTree, tStoixeiouDda1);
            if (tStoixeiouDda) {
                printf("found\n");
                E->DataArray[tStoixeiouDda->arrayIndex].outbound++;
                free(tStoixeiouDda1);
            } else {
                printf("not found\n");
            }
        }
        if (destinationAirport != NULL) {
            TStoixeiouDDA *tStoixeiouDda1 = (TStoixeiouDDA *) malloc(sizeof(TStoixeiouDDA));
            TSDDA_setKey(&(tStoixeiouDda1->key), atoi(destinationAirport));
            printf("The 4th value is: %s\n", destinationAirport);
            TStoixeiouDDA *tStoixeiouDda = rbt_find_node(E->redBlackTree, tStoixeiouDda1);
            if (tStoixeiouDda) {
                printf("found\n");
                E->DataArray[tStoixeiouDda->arrayIndex].inbound++;
                free(tStoixeiouDda);
            } else {
                printf("not found\n");
            }
        }
    }

    fclose(fp);
//    rbt_find_node(E->redBlackTree,)

}
