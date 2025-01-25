#pragma once

#include "low.h"
#include "global.h"
#include "colors.h"


// Local
int OBECNA_LOKACJA;



// IPCS
void __zamknij_baseny_handler(){
    switch(OBECNA_LOKACJA){
        case LOKACJA_KASA:
            printf_clr(blue, "%d: klient opuscil kase - zamkniecie kompleksu\n", getpid());
            break;
        
        case LOKACJA_BASEN:
            printf_clr(blue, "%d: klient opuscil basen\n", getpid());
            break;
    }

    printf_clr(blue, "%d: klient opuscil kompleks basenow\n", getpid());   
    exit(EXIT_SUCCESS);
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
        perror("klient - msgget");
        exit(EXIT_FAILURE);
    }
}

void setup(){
    srand(getpid());
    __set_zamknij_baseny_handler();
    __access_kasa_msq();
}