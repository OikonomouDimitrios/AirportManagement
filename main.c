#include <stdio.h>
#include <string.h>
#include "Evretirio.h"
#include <windows.h>

#define MAX_LINE_LENGTH 1024

int write_results_to_file(char filename[], int total_number_of_flights, double airports_insertion_times[],
                          double total_insertion_time, double total_time_of_search, int total_routes,
                          int found, int not_found, double mean_time_of_search);

int main() {

    EvrPtr E = EvrConstruct(MAXSIZE);
    char airports_random_path[] = "../data/airportsWinRandom.txt";
    char airports_sorted_path[] = "../data/airportsWinSorted.txt";

    char routes_file_path[] = "../data/routesWin.txt";


    int total_number_of_flights;
    double airports_random_insertion_times[4];
    double total_random_insertion_time;
    int result = Evr_GetFlightsFromFile(E, airports_random_path, &total_number_of_flights,
                                        airports_random_insertion_times,
                                        &total_random_insertion_time);

    if (result == -1) {
        printf("Error: could not open file to get flights.\n");
    }

    int found = 0, not_found = 0, total_routes;
    double mean_time_of_random_search, total_time_of_random_search;
    result = Evr_GetRoutesFromFile(E, routes_file_path, &found, &not_found, &total_routes, &mean_time_of_random_search,
                                   &total_time_of_random_search);
    if (result == -1) {
        printf("Error: could not open file to get routes.\n");
    }

    write_results_to_file("../data/OUTPUTRandomBST.txt", total_number_of_flights, airports_random_insertion_times,
                          total_random_insertion_time, total_time_of_random_search, total_routes, found, not_found,
                          mean_time_of_random_search);

    //   Evr_Destruct(&E);
    return 0;
}

int write_results_to_file(char filename[], int total_number_of_flights, double airports_insertion_times[],
                          double total_insertion_time, double total_time_of_search, int total_routes,
                          int found, int not_found, double mean_time_of_search) {
    // Open the file for appending
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;  // Return an error code
    }

    // Write the elapsed time to the file
    fprintf(fp, "-----------------START OF OUTPUT----------------------------\n");
    fprintf(fp,
            "total number of flights %d. Time elapsed after 511 entries %lf, after 1023 entries %lf, after 2047 entries %lf after 4095 entries %lf, total insertion time %lf\n",
            total_number_of_flights,
            airports_insertion_times[0],
            airports_insertion_times[1],
            airports_insertion_times[2],
            airports_insertion_times[3],
            total_insertion_time);
    fprintf(fp, "Total time of search %lf. Total routes %d.Total inbound/outbound Found %d. Not found %d."
                "Mean time of search for each airport %E\n",
            total_time_of_search, total_routes,
            found, not_found, mean_time_of_search);

    fprintf(fp, "-----------------END OF OUTPUT------------------------------\n");

    fclose(fp);  // Close the file
    return 0;     // Return 0 for success
}
