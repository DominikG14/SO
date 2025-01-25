#pragma once

#include "global.h"


// Processes
const char* PS_CLIENT_PATH = "build/client";
const char* PS_CLIENT_NAME  = "./build/client";

const char* PS_CASHIER_PATH = "build/cashier"; 
const char* PS_CASHIER_NAME  = "./build/cashier";

const char* PS_LIFEGUARD_PATH = "build/lifeguard"; 
const char* PS_LIFEGUARD_NAME  = "./build/lifeguard";


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