#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>


enum THREADS__STATES {
    THREADS__SUCCESS = 0,
    THREADS__FAILURE = -1,
};


void join_threads(){ // TODO: Implement
}


pthread_t new_thread(void* (*thread_func)(void*), void* thread_args){
    pthread_t tid;

    int status = pthread_create(&tid, NULL, thread_func, thread_args);
    if(status == THREADS__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return tid;
}