#pragma once

#include "global.h"


// Processes
const char* PS_CLIENT_PATH = "build/klient";
const char* PS_CLIENT_NAME  = "./build/klient";

const char* PS_CASHIER_PATH = "build/kasjer"; 
const char* PS_CASHIER_NAME  = "./build/kasjer";

const char* PS_LIFEGUARD_PATH = "build/ratownik"; 
const char* PS_LIFEGUARD_NAME  = "./build/ratownik";


// Message Queue
struct MSQ_BUFFER {
	long mtype;
	int  mvalue;
} typedef MsqBuffer;


// Threads
pthread_t new_thread(void* (*thread_func)(void*), void* thread_args){
    pthread_t tid;

    int status = pthread_create(&tid, NULL, thread_func, thread_args);
    if(status == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return tid;
}