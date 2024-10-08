#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "../inc/users.h"
#include "../inc/parser.h"
#include "../inc/flights.h"
#include "../inc/passengers.h"
#include "../inc/reservations.h"
#include "../inc/validation.h"
#include "../inc/stats.h"
#include "../inc/user_stat.h"
#include "../inc/hotel_stats.h"
#include "../inc/airport_stats.h"

/*
* Struct user
* Guarda os dados dos utilizadores
*/
struct user{
    char *id;
    char *name;
    char *email;
    char *phone_number;
    char *birth_date; // aaaaa/MM/dd com horas hh:mm:ss
    char *sex;
    char *passport;
    char *country_code;
    char *address;
    char *account_creation;
    char *pay_method;
    char *account_status;
};

int start_user_process(char *line, CATALOGO_USER *cat_user, CATALOGO_INVALID *cat_invalids){
    USER *u = create_User(line);
    if(user_validation(u) == 0){
        addUser(cat_user, u->id, u);
        return 0;
    }
    else{
        //ou fazer so replace (evitar mais 1 comparação)
        if(strcmp("",u->id) != 0)
            addInvalidUser(cat_invalids, getID(u), "invalid");
            //g_hash_table_insert(invalid_users, getID(u), "invalid");
        kill_user(u);
        return 1;
    }
    return 1;
}
/*
    Função responsavel por criar uma strut USER e colocar la os dados necessários
*/
USER *create_User(char *line){

    USER *u = malloc(sizeof(USER));

    u->id = strdup(strsep(&line, ";"));
    u->name = strdup(strsep(&line, ";"));
    u->email = strdup(strsep(&line, ";"));
    u->phone_number = strdup(strsep(&line, ";"));
    u->birth_date = strdup(strsep(&line, ";"));
    u->sex = strdup(strsep(&line, ";"));
    u->passport = strdup(strsep(&line, ";"));
    u->country_code = strdup(strsep(&line, ";"));
    u->address = strdup(strsep(&line, ";"));
    u->account_creation = strdup(strsep(&line, ";"));
    u->pay_method = strdup(strsep(&line, ";"));
    u->account_status = strdup(strsep(&line, "\n"));

    return u;
}

/*
    Função que liberta o espaço alocado pela função create_USER
*/
void kill_user(void *user){
    USER *u = user;

    free(u->id);
    free(u->name);
    free(u->email);
    free(u->phone_number);
    free(u->birth_date);
    free(u->sex);
    free(u->passport);
    free(u->country_code);
    free(u->address);
    free(u->account_creation);
    free(u->pay_method);
    free(u->account_status);
    free(u);
}

//GETTERS
char *getID(USER *u){
    return strdup(u->id);
}

char *getName(USER *u){
    return strdup(u->name);
}

char *getEmail(USER *u){
    return strdup(u->email);
}

char *getPhoneNumber(USER *u){
    return strdup(u->phone_number);
}

char *getBirthDate(USER *u){
    return strdup(u->birth_date);
}

char *getSex(USER *u){
    return strdup(u->sex);
}

char *getPassport(USER *u){
    return strdup(u->passport);
}

char *getCountryCode(USER *u){
    return strdup(u->country_code);
}

char *getAddress(USER *u){
    return strdup(u->address);
}

char *getAccountCreation(USER *u){
    return strdup(u->account_creation);
}

char *getPayMethod(USER *u){
    return strdup(u->pay_method);
}

char *getAccountStatus(USER *u){
    return strdup(u->account_status);
}