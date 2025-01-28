#pragma once

#include "low.h"
#include "global.h"
#include "colors.h"
#include "logging.h"
#include "config.h"


// -------------------- Local --------------------
bool CASH_OPEN = true;


// -------------------- IPCS --------------------
void __close_complex_handler(){
    CASH_OPEN = false;
}


void __set_close_complex_handler(){
    struct sigaction handler;
    handler.sa_handler = __close_complex_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIG_CLOSE_COMPLEX, &handler, 0);
}


void __access_cash_msq(){
    key_t key = get_key(KEY_CASH_MSQ);
    if((CASH_MSQID = msgget(key, 0600)) == FAILURE){
        perror("kasjer - msgget");
        exit(EXIT_FAILURE);
    }
}


void setup(){
    SEM_OPERATE.sem_flg=0;
    __set_close_complex_handler();
    set_config_vars();
    __access_cash_msq();
}