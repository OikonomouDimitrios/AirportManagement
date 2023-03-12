#include <stdio.h>
#include <string.h>
#include "Evretirio.h"
#include <windows.h>

#define MAX_LINE_LENGTH 1024

int handle_data_for_Evretirio(EvrPtr evrPtr, char airports_path[], char routes_path[], char output_path[]);

int main() {
    char routes_file_path[] = "../data/routesWin.txt";

    char airports_random_path[] = "../data/airportsWinRandom.txt";
    char random_bst_output_file_path[] = "../output_files/OUTPUTRandomBST.txt";

    char airports_sorted_path[] = "../data/airportsWinSorted.txt";
    char sorted_bst_output_file_path[] = "../output_files/OUTPUTSortedBST.txt";

    EvrPtr E_rnd = Evr_Construct(MAXSIZE);
    int result = handle_data_for_Evretirio(E_rnd, airports_random_path, routes_file_path, random_bst_output_file_path);
    if (result == -1) {
        printf("error in handling data for evretirio");
    }
    EvrPtr E_sorted = Evr_Construct(MAXSIZE);
    result = handle_data_for_Evretirio(E_sorted, airports_sorted_path, routes_file_path, sorted_bst_output_file_path);
    if (result == -1) {
        printf("error in handling data for evretirio");
    }
    Evr_Destruct(&E_rnd);
    Evr_Destruct(&E_sorted);
    return 0;
}

int handle_data_for_Evretirio(EvrPtr evrPtr, char airports_path[], char routes_path[], char output_path[]) {
    int total_number_of_flights = 0;
    double airports_random_insertion_times[4];
    double total_random_insertion_time = 0;
    int result = Evr_GetFlightsFromFile(evrPtr, airports_path, &total_number_of_flights,
                                        airports_random_insertion_times,
                                        &total_random_insertion_time);

    if (result == -1) {
        printf("Error: could not open file to get flights.\n");
        return -1;
    }

    int found = 0, not_found = 0, total_routes = 0;
    double mean_time_of_search_for_found_entries = 0, mean_time_of_search_total = 0, total_time_of_search = 0;
    result = Evr_GetRoutesFromFile(evrPtr, routes_path, &found, &not_found, &total_routes,
                                   &mean_time_of_search_for_found_entries, &mean_time_of_search_total,
                                   &total_time_of_search);
    if (result == -1) {
        printf("Error: could not open file to get routes.\n");
        return -1;
    }

    result = Evr_WriteResultsToFile(output_path, total_number_of_flights,
                                    airports_random_insertion_times,
                                    total_random_insertion_time, total_time_of_search, total_routes, found,
                                    not_found,
                                    mean_time_of_search_for_found_entries, mean_time_of_search_total);
    if (result == -1) {
        printf("Error opening file.\n");
        return -1;
    }
    return 0;
}

