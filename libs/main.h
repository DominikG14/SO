#pragma once

#include "colors.h"
#include "global.h"
#include "low.h"


// Processes data
int PID_KASJER;

int PID_KLIENCI[1000];
int ILOSC_KLIENTOW = 0;


// IPCS
void cleanup(){
    msgctl(KASA_MSQID, IPC_RMID, NULL);
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

void __create_kasa_msq(){
    key_t key = ftok(".", KASA_KEY);
    if((KASA_MSQID = msgget(key, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - kasa");
        exit(EXIT_FAILURE);
    }
}

void setup(){
    __set_shutdown_handler();
    __create_kasa_msq();
}