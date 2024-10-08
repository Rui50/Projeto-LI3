#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/reservations.h"
#include "../inc/users.h"
#include "../inc/flights.h"
#include "../inc/passengers.h"
#include "../inc/validation.h"
#include "../inc/stats.h"
#include "../inc/user_stat.h"
#include "../inc/hotel_stats.h"
#include "../inc/airport_stats.h"
#include "../inc/catalogo_user.h"
#include "../inc/catalogo_reservations.h"

/*
    Struct responsavel por guardar dados das reservas
*/
struct reservation{
    char *id;
    char *user_id;
    char *hotel_id;
    char *hotel_name;
    double hotel_stars;
    double city_tax;
    char *adress;
    char *begin_date;
    char *end_date;
    double price_per_night;
    char *includes_breakfast;
    double rating;
};


int start_reservation_process(char *line, CATALOGO_USER *cat_users, CATALOGO_RESERVATIONS *cat_reservas, STATS *stats, CATALOGO_INVALID *cat_invalids){
    
    RESERVATION *r = create_Reservation(line);

    char *userID = getUserID_reservartion(r);

    if(!cointains_invalid_user(cat_invalids, userID) && reservation_validation(r) == 0){
    //if(!(g_hash_table_contains(invalid_users, r->user_id)) && reservation_validation(r) == 0){
        addReservations(cat_reservas, r->id, r);

        if (contains_stat(stats, r->user_id))
            create_user_stat_reservations(r, stats, cat_users);
        if (contains_stat(stats, r->hotel_id)){
            create_hotel_stats(r, stats);
        }
        
        free(userID);
        return 0;
    }

    free(userID);

    kill_reservation(r);
    return 1;
}


/*
    Função responsavel por criar uma strust RESERVATION e inserir os dados
*/
RESERVATION*create_Reservation(char *line){

    RESERVATION *r = malloc(sizeof(RESERVATION));

    r->id = strdup(strsep(&line, ";"));
    r->user_id = strdup(strsep(&line, ";"));
    r->hotel_id = strdup(strsep(&line, ";"));
    r->hotel_name = strdup(strsep(&line, ";"));
    r->hotel_stars = strtod(strsep(&line, ";"), NULL);
    r->city_tax = strtod(strsep(&line, ";"), NULL);
    r->adress = strdup(strsep(&line, ";"));
    r->begin_date = strdup(strsep(&line, ";"));
    r->end_date = strdup(strsep(&line, ";"));
    r->price_per_night = strtod(strsep(&line, ";"), NULL);
    r->includes_breakfast = strdup(strsep(&line, ";"));
    strsep(&line, ";");
    r->rating = strtod(strsep(&line, ";"), NULL);
    strsep(&line, "\n");
    return r;
}

/*
    Função responsavel por libertar o espaço criado pela função create_Reservation
*/
void kill_reservation(void *reservation){
    RESERVATION *r = reservation;

    free(r->id);
    free(r->user_id);
    free(r->hotel_id);
    free(r->hotel_name);
    free(r->adress);
    free(r->begin_date);
    free(r->end_date);
    free(r->includes_breakfast);
    free(r);
}


// GETTERS
char *getID_reservation(RESERVATION *r){
    return strdup(r->id);
}

char *getUserID_reservartion(RESERVATION *r){
    return strdup(r->user_id);
}

char *getHotelID_reservation(RESERVATION *r) {
    return strdup(r->hotel_id);
}

char *getHotelName_reservation(RESERVATION *r) {
    return strdup(r->hotel_name);
}

double getHotelStars_reservation(RESERVATION *r) {
    return r->hotel_stars;
}

double getCityTax_reservation(RESERVATION *r) {
    return r->city_tax;
}

char *getAddress_reservation(RESERVATION *r) {
    return strdup(r->adress);
}

char *getBeginDate_reservation(RESERVATION *r) {
    return strdup(r->begin_date);
}

char *getEndDate_reservation(RESERVATION *r) {
    return strdup(r->end_date);
}

double getPricePerNight_reservation(RESERVATION *r) {
    return r->price_per_night;
}

char *getIncludesBreakfast_reservation(RESERVATION *r) {
    return strdup(r->includes_breakfast);
}

double getRating_reservation(RESERVATION *r) {
    return r->rating;
}