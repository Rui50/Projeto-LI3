#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../inc/tests.h"
#include "../../inc/stats_needed.h"
#include "../../inc/stats.h"

void execute_tests(char *data_input, char *test_path){
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE * file ;
    file = fopen(data_input , "r");

    int i, first_error = 0;
    FILE *executed_queries, *results;
    for (i = 1; (read = getline(&line, &len, file)) != -1; ++i){
        line[read-1] = '\0';
        char *query = strdup(strsep(&line, " "));

        char *path_executed_queries = malloc(sizeof(char) * 100);
        sprintf(path_executed_queries, "Resultados/command%d_output.txt", i);
        executed_queries = fopen(path_executed_queries, "r");
        //printf("%s\n", path_executed_queries);

        if (executed_queries == NULL)
            continue; 
            
        char *path_results = malloc(sizeof(char) * 100);
        sprintf(path_results, "%s/command%d_output.txt", test_path, i);
        //printf("%s\n", path_results);
        results = fopen(path_results, "r");

        char *results_line = NULL;
        size_t results_len = 0;

        char *tests_line = NULL;
        size_t tests_len = 0;

        for (int a = 1; getline(&results_line, &results_len, executed_queries) != -1 && getline(&tests_line, &tests_len, results) != -1; a++){
            //printf("%s\n", results_line);
            //printf("%s\n", tests_line);
            if (strcmp(results_line, tests_line)){
                if (first_error == 0){
                    printf("\n");
                    printf("Erro no ficheiro %s na linha %d\n", path_executed_queries, a);
                    printf("Esperado: %s", tests_line);
                    printf("Obtido: %s", results_line);
                    first_error = 1;
                }
            }
        }

        fclose(executed_queries);
        fclose(results);
        free(results_line);
        free(tests_line);
        free(path_executed_queries);
        free(path_results);
        free(query);
    }
    if (first_error == 0)
        printf("Passou no teste com sucesso\n");
    fclose(file);
}

// fazer uma função geral para isto
void run_test(char *files_path, char *data_input){
    CATALOGO_USER *cat_users = create_Catalogo_User();
    CATALOGO_FLIGHTS *cat_flights = create_Catalogo_flights();
    CATALOGO_PASSENGER *cat_passengers = create_catalogo_Passenger();
    CATALOGO_RESERVATIONS *cat_reservations = create_Catalogo_Reservations();
    CATALOGO_INVALID *cat_invalids = create_Catalogo_invalids();

    //criaçao das stats
    STATS *stats = create_stats();    
    start_stats_needed(stats, data_input);
    
    //parsing dos ficheiros
    parse_files_users_teste(files_path, cat_users, cat_invalids);
    printf("User parsing and validation complete\n");
    parse_files_flights_teste(files_path, cat_flights, stats, cat_invalids);
    printf("Flights parsing and validation complete\n");
    parse_files_passengers_teste(files_path, stats, cat_passengers, cat_users, cat_flights, cat_invalids);
    printf("Passengers parsing and validation complete\n");
    parse_files_reservations_test(files_path, stats, cat_reservations, cat_users, cat_invalids);
    printf("Reservations parsing and validation complete\n");

    // Realizar as queries
    handle(data_input, cat_users, cat_flights, cat_passengers, cat_reservations, stats, cat_invalids, 1);

    // Libertar Memoria
    destroy_catalogo_invalids(cat_invalids);

    destroy_catalogo_users(cat_users);
    destroy_catalogo_flights(cat_flights);
    destroy_catalogo_reservations(cat_reservations);
    destroy_catalogo_passengers(cat_passengers);
    destroy_stats(stats);
}