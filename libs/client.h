#pragma once

#include "random.h"



const int CLIENT__HAS_CHILD_PERC = 29;
const int CLIENT__SWIM_CAP_PREC = 30;


struct Client {
    int age;
    bool swim_cap_on;
} typedef Client;


void setup(){
    srand(getpid());
}


bool rand_child(int client_age, int child_age){
    int child_perc = rand_int(1, 100);
    if(client_age - child_age >= 18 && child_perc <= CLIENT__HAS_CHILD_PERC){
        return true;
    }

    return false;
}


bool rand_swim_cap(){
    int swim_cap_perc = rand_int(1, 100);
    if(swim_cap_perc <= CLIENT__SWIM_CAP_PREC){
        return true;
    }

    return false;
}