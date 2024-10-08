#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "../inc/batch.h"
#include "../inc/parser.h"
#include "../inc/catalogo_user.h"
#include "../inc/catalogo_reservations.h"
#include "../inc/catalogo_flights.h"
#include "../inc/catalogo_invalids.h"
#include "../inc/utils.h"
#include "../inc/handle.h"
#include "../inc/stats_needed.h"
#include <sys/resource.h>


void print_memory_usage() {
    struct rusage usage;

    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        long megabytes = usage.ru_maxrss / 1024; //  KB to MB
        printf("Memoria Usada: %ld megabytes\n", megabytes);
    } 
}

void batch(char *files_path, char *data_input){

        //criacao dos catalogos
        CATALOGO_USER *cat_users = create_Catalogo_User();
        CATALOGO_FLIGHTS *cat_flights = create_Catalogo_flights();
        CATALOGO_PASSENGER *cat_passengers = create_catalogo_Passenger();
        CATALOGO_RESERVATIONS *cat_reservations = create_Catalogo_Reservations();
        CATALOGO_INVALID *cat_invalids = create_Catalogo_invalids();

        //criaçao das stats
        STATS *stats = create_stats();    
        start_stats_needed(stats, data_input);
        
        printf("STARTING FILES PARSING\n");
        
        //parsing dos ficheiros
        parse_files_users_teste(files_path, cat_users, cat_invalids);
        printf("User Validation and Parsing Sucessfull\n");
        parse_files_flights_teste(files_path, cat_flights, stats, cat_invalids);
        printf("Flights Validation and Parsing Sucessfull\n");
        parse_files_passengers_teste(files_path, stats, cat_passengers, cat_users, cat_flights, cat_invalids);
        printf("Passengers Validation and Parsing Sucessfull\n");
        parse_files_reservations_test(files_path, stats, cat_reservations, cat_users, cat_invalids);
        printf("Reservations Validation and Parsing Sucessfull\n");

        //print_memory_usage();

        //apenas necessários para o parsing
        destroy_stats_needed(stats);

        // Realizar as queries
        handle(data_input, cat_users, cat_flights, cat_passengers, cat_reservations, stats, cat_invalids, 0);

        // Libertar Memoria
        destroy_catalogo_invalids(cat_invalids);
        destroy_catalogo_users(cat_users);
        destroy_catalogo_flights(cat_flights);
        destroy_catalogo_reservations(cat_reservations);
        destroy_catalogo_passengers(cat_passengers);
        destroy_stats(stats);

        print_memory_usage();
}