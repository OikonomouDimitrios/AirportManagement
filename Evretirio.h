#ifndef __Evr__
#define __Evr__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TSEvr.h"
#include "TSDDA.h"
#include <windows.h>
#define MAX_LINE_LENGTH 1024

#define MAXSIZE 72000

typedef enum {
    INBOUND,
    OUTBOUND
} FlightType;

typedef struct EvrNode *EvrPtr;

EvrPtr Evr_Construct(int MaxSize);    /* gia erwthma 1 */
int Evr_Destruct(EvrPtr *E);        /* gia erwthma 5 */

int Evr_Insert(EvrPtr E, TStoixeiouEvr Data); /* gia erwthma 2 */
int EvrSearch(EvrPtr E, keyType key, FlightType InOut, int *found); /* gia erwthma 3 */

int Evr_GetFlightsFromFile(EvrPtr E, char file_path[], int *total_number_of_flights, double interval_times[],
                           double *total_insertion_time);

int Evr_GetRoutesFromFile(EvrPtr E, char file_path[], int *total_found, int *total_not_found, int *total_routes,
                          double *mean_time_of_search_for_found_entries, double *mean_time_of_search_for_all_entries,
                          double *total_time_of_search_operation);


int Evr_WriteResultsToFile(char filename[], int total_number_of_flights, double airports_insertion_times[],
                           double total_insertion_time, double total_time_of_search, int total_routes, int found,
                           int not_found, double mean_time_of_search_for_found_entries,
                           double mean_time_of_search_total);

#endif
