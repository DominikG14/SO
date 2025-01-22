#pragma once

#include "random.h"



const int CLIENT_HAS_CHILD_PERC = 100;
const int CLIENT_SWIM_CAP_PREC = 30;

// Expres how many (TIME_PER_SEC)
const int CLIENT_MIN_SWIM_TIME = 3;
const int CLIENT_MAX_SWIM_TIME = 10;

const int CLIENT_MIN_AGE = 60;
const int CLIENT_MAX_AGE = 70;

const int CHILD_MIN_AGE = 1;
const int CHILD_MAX_AGE = 5;


struct Child {
    pthread_t tid;

    int age;
    bool swim_cap_on;
    bool diaper_on;

    bool WAIT_IN_CASH;
    bool SWIM_IN_POOL;
} typedef Child;


struct Client {
    int age;
    bool swim_cap_on;
} typedef Client;


bool rand_child(int client_age, int child_age){
    int child_perc = rand_int(1, 100);
    if(client_age - child_age >= 18 && child_perc <= CLIENT_HAS_CHILD_PERC){
        return true;
    }

    return false;
}


bool rand_swim_cap(){
    int swim_cap_perc = rand_int(1, 100);
    if(swim_cap_perc <= CLIENT_SWIM_CAP_PREC){
        return true;
    }

    return false;
}


int rand_swim_time(){
    return rand_int(CLIENT_MIN_SWIM_TIME, CLIENT_MAX_SWIM_TIME);
}