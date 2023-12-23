#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../inc/passengers.h"
#include "../inc/users.h"
#include "../inc/flights.h"
#include "../inc/reservations.h"
#include "../inc/validation.h"
#include "../inc/stats.h"
#include "../inc/user_stat.h"
#include "../inc/hotel_stats.h"
#include "../inc/airport_stats.h"
#include "../inc/catalogo_flights.h"

/*
    Struct responsavel por guardar os dados dos passageiros
*/
struct passenger{
    char *flight_id;
    char *user_id;
};

//int start_passenger_process(char *line,CATALOGO_PASSENGER *cat_passenger, CATALOGO_INVALID *cat_invalids, STATS *stats, CATALOGO_FLIGHTS *cat_flights, CATALOGO_USER *cat_users){
int start_passenger_process(char *line,CATALOGO_PASSENGER *cat_passenger, CATALOGO_INVALID *cat_invalids, STATS *stats, CATALOGO_FLIGHTS *cat_flights, CATALOGO_USER *cat_users, GHashTable *stats_need){

    PASSENGER *p = create_Passenger(line);
    
    char *flightID = get_FlightID_passenger(p);
    char *userID = getID_passenger(p);
    if(!cointains_invalid_user(cat_invalids, userID) && !cointains_invalid_flight(cat_invalids, flightID) && strcmp("",p->flight_id) != 0){

    //if(!g_hash_table_contains(invalid_flights, p->flight_id) && !g_hash_table_contains(invalid_users, p->user_id) && strcmp("",p->flight_id) != 0){
        addPassenger(cat_passenger, p);
        
        //printf("%s\n", getFlightDestination(f));
        if(g_hash_table_contains(stats_need, p->user_id))
            create_user_stat_flights(p, get_user_stats(stats), cat_users, cat_flights);
        create_airport_stat_passenger(p, get_airport_stats(stats), cat_flights);

        free(flightID);
        free(userID);

        return 0;
    }

    free(flightID);
    free(userID);
    kill_Passenger(p);

    return 1;
}
/*
    Função responsavel pela criação de uma struct PASSENGER e atribuição dos seus dados
*/
PASSENGER *create_Passenger(char *line){

    PASSENGER *p = malloc(sizeof(PASSENGER));

    p->flight_id = strdup(strsep(&line, ";"));
    p->user_id = strdup(strsep(&line, "\n"));

    return p;
}


// GETTERS
char *get_FlightID_passenger(PASSENGER *p){
    return strdup(p->flight_id);
}

char *getID_passenger(PASSENGER *p){
    return strdup(p->user_id);
}

/*
    Função que liberta o espaço alocado na funcao create_passenger
*/
void kill_Passenger(void *passenger){
    PASSENGER *p = passenger;

    free(p->flight_id);
    free(p->user_id);
    free(p);
}


/*
    Função responsavel por dar free ao array completo
*/
void freeGArray(GArray *garray) {
    g_assert(garray != NULL);

    for (guint i = 0; i < garray->len; i++) {
        PASSENGER* p = g_array_index(garray, PASSENGER*, i);
        free(p->flight_id);
        free(p->user_id);
        free(p);
    }

    g_array_free(garray, TRUE);
}


/*
    Função que faz o parsing dos passageiros
    Apenas coloca na hashtable dos passageiros se ambos utilizador->id e flight->id forem válidos
    Aqui é também criadas/atualizads stats para o user em relação aos flights assim como para os aeroportos
*/
/*GArray *parse_files_passengers(char *path, STATS*stats, GHashTable *users, GHashTable *flights, GHashTable *invalid_users, GHashTable *invalid_flights) {
    char *path_passengers = malloc(sizeof(char) * 70);
    strcpy(path_passengers, path);
    strcat(path_passengers, "/passengers.csv");
    char *path_passengers_erros = "Resultados/passengers_errors.csv";

    char *line = NULL;
    size_t len = 0;

    GArray *passengers = g_array_new(FALSE, TRUE, sizeof(PASSENGER *));

    FILE *file = fopen(path_passengers, "r");
    FILE *file_errors = fopen(path_passengers_erros, "w");

    //cabeçalho ficheiro de erros
    fprintf(file_errors, "flight_id;user_id\n");

    if (file == NULL) {
        printf("Unable to open the file.\n");
        return NULL;
    }

    getline(&line, &len, file);

    while ((getline(&line, &len, file)) != -1) {

        char *temp = strdup(line);
        PASSENGER *passenger = create_Passenger(line);

        if((g_hash_table_contains(invalid_users, passenger->user_id) == FALSE) && !(g_hash_table_contains(invalid_flights, passenger->flight_id)) && strcmp("",passenger->flight_id) != 0){
            g_array_append_val(passengers, passenger);

            create_user_stat_flights(passenger, get_user_stats(stats), users, flights);
            create_airport_stat_passenger(passenger, get_airport_stats(stats), flights);
        }
        else{
            fprintf(file_errors, "%s", temp);
            
            kill_Passenger(passenger);
        }
        free(temp);
    }

    printf("Passenger Validation and Parsing Successfull\n");
    free(line);
    free(path_passengers);
    fclose(file_errors);
    fclose(file);

    return passengers;
}*/