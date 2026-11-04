#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include "sync_car.h"

void station_init(struct station *station){

    if(station == NULL){
        fprintf(stderr, "erro: station está vazia\n");
        exit(1);
    }

    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->passager_boarded, NULL);

    station->waiting = 0;
    station->capacity = 0;
    station->seats_taken = 0;
    station->boarded = 0;
}

void station_load_car(struct station *station, int count){

    if(station == NULL){
        return;
    }

    pthread_mutex_lock(&station->lock);

    station->seats_taken = 0;
    station->boarded = 0;

    if(count == 0 || station->waiting == 0){
        station->capacity = 0;
        pthread_mutex_unlock(&station->lock);
        return;
    }

    station->capacity = count;
    pthread_cond_broadcast(&station->car_arrived);

    while(station->boarded < station->seats_taken || (station->waiting > 0 && station->seats_taken < station->capacity)) {
        pthread_cond_wait(&station->passager_boarded, &station->lock);
    }

    station->capacity = 0;
    pthread_mutex_unlock(&station->lock);
}

void station_wait_for_car(struct station *station){

    if(station == NULL){
        return;
    }

    if(station == NULL){
        return;
    }

    pthread_mutex_lock(&station->lock);

    station->waiting++;

    while(station->capacity == 0 || station->seats_taken == station->capacity){
        pthread_cond_wait(&station->car_arrived, &station->lock);
    }

    station->seats_taken++;
    station->waiting--;

    pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station){

    if(station == NULL){
        return;
    }

    pthread_mutex_lock(&station->lock);

    station->boarded++;
    pthread_cond_signal(&station->passager_boarded);

    pthread_mutex_unlock(&station->lock);
}
