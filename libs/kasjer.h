#pragma once

#include "low.h"
#include "global.h"
#include "colors.h"


// Local
bool KASA_OTWARTA= true;


// IPCS
void __zamknij_baseny_handler(){
    KASA_OTWARTA = false;
}

void __set_zamknij_baseny_handler(){
    struct sigaction handler;
    handler.sa_handler = __zamknij_baseny_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIG_ZAMKNIJ_BASENY, &handler, 0);
}

void __access_kasa_msq(){
    key_t key = ftok(".", KASA_KEY);
    if((KASA_MSQID = msgget(key, 0600)) == FAILURE){
        perror("kasjer - msgget");
        exit(EXIT_FAILURE);
    }
}

void setup(){
    __set_zamknij_baseny_handler();
    __access_kasa_msq();

}