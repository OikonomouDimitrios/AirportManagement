#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "Evretirio.h"
#include <windows.h>

#include "RedBlackTreeFunctions.h" /* h Ylopoihsh sas toy R/B */

double calculateElapsedTime(LARGE_INTEGER start_time, LARGE_INTEGER end_time, LARGE_INTEGER frequency);

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

int EvrSearch(EvrPtr E, keyType key, FlightType InOut, int *found) {
    TStoixeiouDDA *tStoixeiouDda = rbt_find_node(E->redBlackTree, &key);
    if (tStoixeiouDda) {
        *found = 1;
//        printf("found key %d!\n", key);
        if (tStoixeiouDda->arrayIndex > MAXSIZE) {
            return -1;
        }
        if (InOut == INBOUND) {
            E->DataArray[tStoixeiouDda->arrayIndex].inbound++;
        } else {
            E->DataArray[tStoixeiouDda->arrayIndex].outbound++;
        }
    } else {
        *found = 0;
    }
    return 0;

}


int Evr_PrintAll(EvrPtr E, FILE *out, int *counter) {
    rbt_print_tree(&(E->redBlackTree), counter);
}

int Evr_Destruct(EvrPtr *E) {
    rbt_free(&((*E)->redBlackTree));
    for (int i = 0; i < (*E)->Index; i++) {
        free((*E)->DataArray[i].name);
        free((*E)->DataArray[i].city);
        free((*E)->DataArray[i].country);
        free((*E)->DataArray[i].iata);
        free((*E)->DataArray[i].icao);
    }
    free((*E)->DataArray);
    free(*E);
}

int Evr_GetFlightsFromFile(EvrPtr E, char file_path[], int *total_number_of_flights, double interval_times[],
                           double *total_insertion_time) {
    LARGE_INTEGER start_time, interval_time, end_time, frequency;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    FILE *fp = fopen(file_path, "r");
    char line[MAX_LINE_LENGTH];
    const char delimiter[] = ";\r";

    if (fp == NULL) {
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        (*total_number_of_flights)++;
        if (*total_number_of_flights >= 511) {
            int number_of_iterations[] = {511, 1023, 2047, 4095};
            int index = -1;
            for (int i = 0; i < sizeof(number_of_iterations) / sizeof(number_of_iterations[0]); i++) {
                if (*total_number_of_flights == number_of_iterations[i]) {
                    index = i;
                    break;
                }
            }
            if (index >= 0) {
                QueryPerformanceCounter(&interval_time);
                interval_times[index] = calculateElapsedTime(start_time, interval_time, frequency);
            }
        }
        char *token;
        int field_index = 0;
        token = strtok(line, delimiter);
        token[strcspn(token, "\n")] = '\0';
        TStoixeiouEvr *tStoixeiouEvr = malloc(sizeof(TStoixeiouEvr));
        while (token != NULL && field_index < 6) {
            token[strcspn(token, "\n")] = '\0';
            TSEvr_setValuesFromFileParsing(tStoixeiouEvr, field_index, token);
            field_index++;
            token = strtok(NULL, delimiter);
        }

        EvrInsert(E, *tStoixeiouEvr);
    }
    QueryPerformanceCounter(&end_time);
    *total_insertion_time = calculateElapsedTime(start_time, end_time, frequency);
    fclose(fp);
}

int Evr_GetRoutesFromFile(EvrPtr E, char file_path[], int *total_found, int *total_not_found, int *total_routes,
                          double *mean_time_of_search_for_found_entries, double *mean_time_of_search_for_all_entries,
                          double *total_time_of_search_operation) {
    FILE *fp = fopen(file_path, "r");
    char line[MAX_LINE_LENGTH];
    if (fp == NULL) {
        printf("Error: could not open file %s\n", file_path);
        return -1;
    }
    LARGE_INTEGER start_time, end_time, start_search_time, end_search_time, frequency;

    QueryPerformanceFrequency(&frequency);
    double totalSearchTimeForFoundEntries = 0, totalSearchTimeForAllEntries = 0;

    QueryPerformanceCounter(&start_time);

    while (fgets(line, sizeof(line), fp) != NULL) {
        (*total_routes)++;
        char *token = strtok(line, ";");
        int i = 0;
        char *sourceAirport = NULL;
        char *destinationAirport = NULL;
        while (token) {
            i++;
            if (i == 4) {
                sourceAirport = token;
            } else if (i == 6) {
                destinationAirport = token;
            }
            token = strtok(NULL, ";");
        }
        if (sourceAirport != NULL) {
            int found = 0;
            int sourceAirport_int = atoi(sourceAirport);
            QueryPerformanceCounter(&start_search_time);
            EvrSearch(E, sourceAirport_int, OUTBOUND, &found);
            QueryPerformanceCounter(&end_search_time);
            if (found) {
                totalSearchTimeForFoundEntries += calculateElapsedTime(start_search_time, end_search_time, frequency);
                (*total_found)++;
            } else { (*total_not_found)++; };
            totalSearchTimeForAllEntries += calculateElapsedTime(start_search_time, end_search_time, frequency);
        }
        if (destinationAirport != NULL) {
            int found = 0;
            int destinationAirport_int = atoi(destinationAirport);
            QueryPerformanceCounter(&start_search_time);
            EvrSearch(E, destinationAirport_int, INBOUND, &found);
            QueryPerformanceCounter(&end_search_time);
            if (found) {
                totalSearchTimeForFoundEntries += calculateElapsedTime(start_search_time, end_search_time, frequency);
                (*total_found)++;
            } else { (*total_not_found)++; };
            totalSearchTimeForAllEntries += calculateElapsedTime(start_search_time, end_search_time, frequency);
        }
    }
    QueryPerformanceCounter(&end_time);

    *mean_time_of_search_for_found_entries = totalSearchTimeForFoundEntries / (*total_found);
    *mean_time_of_search_for_all_entries = totalSearchTimeForAllEntries / (*total_routes);
    *total_time_of_search_operation = calculateElapsedTime(start_time, end_time, frequency);
    fclose(fp);
    return 0;
}


double calculateElapsedTime(LARGE_INTEGER start_time, LARGE_INTEGER end_time, LARGE_INTEGER frequency) {
    return (double) (end_time.QuadPart - start_time.QuadPart) / (double) frequency.QuadPart;
}

int Evr_WriteResultsToFile(char filename[], int total_number_of_flights, double airports_insertion_times[],
                           double total_insertion_time, double total_time_of_search, int total_routes, int found,
                           int not_found, double mean_time_of_search_for_found_entries,
                           double mean_time_of_search_total) {
    // Open the file for appending
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    // Write the elapsed time to the file
    fprintf(fp, "-----------------START OF OUTPUT----------------------------\n");
    fprintf(fp,
            "Total number of flights %d.\n"
            "Time elapsed after inserting 511 entries %lf seconds.\n"
            "Time elapsed after inserting 1023 entries %lf seconds.\n"
            "Time elapsed after inserting 2047 entries %lf seconds.\n"
            "Time elapsed after inserting 4095 entries %lf seconds.\n"
            "Total insertion time %lf seconds.\n",
            total_number_of_flights,
            airports_insertion_times[0],
            airports_insertion_times[1],
            airports_insertion_times[2],
            airports_insertion_times[3],
            total_insertion_time);
    fprintf(fp,
            "Total search time %lf seconds.\n"
            "Total routes %d.\n"
            "Total inbound/outbound found %d.\n"
            "Total inbound/outbound not found %d.\n"
            "Mean time of search for each found airport %E seconds.\n"
            "Mean time of search for each airport %E seconds.\n",
            total_time_of_search, total_routes,
            found, not_found, mean_time_of_search_for_found_entries, mean_time_of_search_total);

    fprintf(fp, "-----------------END OF OUTPUT------------------------------\n");

    fclose(fp);
    return 0;
}