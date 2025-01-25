#pragma once

#include "colors.h"
#include "global.h"
#include "low.h"


// Processes data
int PID_CASHIER;

int PID_CLIENTS[1000];
int CLIENTS_NUM = 0;


// IPCS
void cleanup(){
    msgctl(CASH_MSQID, IPC_RMID, NULL);
}

void __shutdown_handler(int sig){
    cleanup();
    printf_clr(red, "SHUTDOWN!");
    exit(EXIT_FAILURE);
}

void __set_shutdown_handler(){
    struct sigaction handler;
    handler.sa_handler = __shutdown_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGINT, &handler, 0);
}

void __create_cash_msq(){
    key_t key = ftok(".", KEY_CASH_MSQ);
    if((CASH_MSQID = msgget(key, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - kasa");
        exit(EXIT_FAILURE);
    }
}


void __create_pools(){

}

void setup(){
    __set_shutdown_handler();
    __create_cash_msq();
}