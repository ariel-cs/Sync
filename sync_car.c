#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "sync_car.h"

static void check_station(const struct station *station)
{
    if (station == NULL) {
        fprintf(stderr, "erro: station está vazia\n");
        exit(EXIT_FAILURE);
    }
}

static void check_pthread(int rc)
{
    if(rc != 0) {
        fprintf(stderr, "erro: pthread falhou, codigo %d\n",rc);
        exit(EXIT_FAILURE);
    }
}

void station_init(struct station *station){

    check_station(station);

    check_pthread(pthread_mutex_init(&station->lock, NULL));
    check_pthread(pthread_cond_init(&station->car_arrived, NULL));
    check_pthread(pthread_cond_init(&station->passenger_boarded, NULL));

    station->waiting = 0;
    station->capacity = 0;
    station->seats_taken = 0;
    station->boarded = 0;
}

void station_load_car(struct station *station, int count){

    check_station(station);

    if (count < 0) {
        fprintf(stderr, "erro: count inválido\n");
        return;
    }

    check_pthread(pthread_mutex_lock(&station->lock));

    station->seats_taken = 0;
    station->boarded = 0;

    if(count == 0 || station->waiting == 0){
        station->capacity = 0;
        check_pthread(pthread_mutex_unlock(&station->lock));
        return;
    }

    station->capacity = count;
    check_pthread(pthread_cond_broadcast(&station->car_arrived));

    while(station->boarded < station->seats_taken || (station->waiting > 0 && station->seats_taken < station->capacity)) {
        check_pthread(pthread_cond_wait(&station->passenger_boarded, &station->lock));
    }

    station->capacity = 0;
    check_pthread(pthread_mutex_unlock(&station->lock));
}

void station_wait_for_car(struct station *station){

    check_station(station);

    check_pthread(pthread_mutex_lock(&station->lock));

    station->waiting++;

    while(station->capacity == 0 || station->seats_taken == station->capacity){
        check_pthread(pthread_cond_wait(&station->car_arrived, &station->lock));
    }

    station->seats_taken++;
    station->waiting--;

    check_pthread(pthread_mutex_unlock(&station->lock));
}

void station_on_board(struct station *station){

    check_station(station);

    check_pthread(pthread_mutex_lock(&station->lock));

    station->boarded++;
    check_pthread(pthread_cond_signal(&station->passenger_boarded));

    check_pthread(pthread_mutex_unlock(&station->lock));
}
