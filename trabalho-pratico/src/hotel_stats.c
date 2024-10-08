#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../inc/hotel_stats.h"

/*
	Struct responsável por guardar os dados das stats dos hoteis
*/
struct hotel_stat{
    char *hotelid;
    double nReservas;
    double somaRatings;
    double avgscore;
    GList *reservasHotel;
};

/*
	Função responsavel por libertar o espaço alocado para uma struct HotelStat
*/
void kill_hotelStat(void *hotelStat){
	HOTEL_STAT *hs = hotelStat;
	
	if(hs != NULL){
		free(hs->hotelid);

		g_list_free(hs->reservasHotel);	
	}
	free(hs);
}

// GETTERS

char *get_hotel_stat_id(HOTEL_STAT *h)
{
	return strdup(h->hotelid);
}
double get_hotel_stat_nReservas(HOTEL_STAT *h)
{
	return h->nReservas;
}

double get_hotel_stat_somaRatings(HOTEL_STAT *h)
{
	return h->somaRatings;
}

double get_hotel_stat_avgscore(HOTEL_STAT *h){
	return h->avgscore;
}

GList *get_hotel_stat_reservasHotel(HOTEL_STAT *h)
{
	return g_list_copy(h->reservasHotel);
}



/*
	Função responsavel por criar as stats para cada hotel,
	Cria uma stat nova caso nao exista,
	Caso ja haja stat criada para o hotel irá dar update aos seus campos
*/
void create_hotel_stats(RESERVATION *r, STATS *stats)
{
	char *hotelID = getHotelID_reservation(r);	
	
	HOTEL_STAT *hstat = get_stat_hotel(stats, hotelID);

    double rating = getRating_reservation(r);
		if (hstat == NULL)
		{
			HOTEL_STAT *hotel_stat = malloc(sizeof(HOTEL_STAT));
            
			hotel_stat->hotelid = hotelID;
            hotel_stat->nReservas = 1;
            hotel_stat->somaRatings = rating;
			hotel_stat->avgscore = rating;
            hotel_stat->reservasHotel = g_list_prepend(NULL,r);
			addHotelStat(stats, hotel_stat, hotelID);
		}
		else{
			hstat->nReservas += 1;
            hstat->somaRatings += rating;
            hstat->avgscore = hstat->somaRatings/hstat->nReservas;
            hstat->reservasHotel = g_list_prepend(hstat->reservasHotel,r);
			free(hotelID);
        }
		
}

