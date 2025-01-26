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
    // Cash
    msgctl(CASH_MSQID, IPC_RMID, NULL);

    // Olimpic pool
    msgctl(OLIMPIC_POOL_MSQID, IPC_RMID, NULL);
    semctl(OLIMPIC_POOL_SEMID, 0, IPC_RMID, NULL);
    shmctl(OLIMPIC_POOL_SHMID, IPC_RMID, NULL);

    // Leisure pool
    msgctl(LEISURE_POOL_MSQID, IPC_RMID, NULL);
    semctl(LEISURE_POOL_SEMID, 0, IPC_RMID, NULL);
    shmctl(LEISURE_POOL_SHMID, IPC_RMID, NULL);

    // Paddling pool
    msgctl(PADDLING_POOL_MSQID, IPC_RMID, NULL);
    semctl(PADDLING_POOL_SEMID, 0, IPC_RMID, NULL);
    shmctl(PADDLING_POOL_SHMID, IPC_RMID, NULL);
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
    key_t key = get_key(KEY_CASH_MSQ);
    if((CASH_MSQID = msgget(key, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - kasa");
        exit(EXIT_FAILURE);
    }
}


void __create_pools(){
    key_t key_msq, key_sem, key_shm;

    // Olimpic pool
    key_msq = get_key(KEY_OLIMPIC_POOL_MSQ);
    key_sem = get_key(KEY_OLIMPIC_POOL_SEM);
    key_shm = get_key(KEY_OLIMPIC_POOL_SHM);

    if((OLIMPIC_POOL_MSQID = msgget(key_msq, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((OLIMPIC_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, IPC_CREAT|0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((OLIMPIC_POOL_SHMID = shmget(key_shm, sizeof(int), IPC_CREAT|0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }


    // Leisure pool
    key_msq = get_key(KEY_LEISURE_POOL_MSQ);
    key_sem = get_key(KEY_LEISURE_POOL_SEM);
    key_shm = get_key(KEY_LEISURE_POOL_SHM);

    if((LEISURE_POOL_MSQID = msgget(key_msq, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((LEISURE_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, IPC_CREAT|0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }
        // For size and age_sum
    if((LEISURE_POOL_SHMID = shmget(key_shm, 2*sizeof(int), IPC_CREAT|0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }


    // Paddling pool
    key_msq = get_key(KEY_PADDLING_POOL_MSQ);
    key_sem = get_key(KEY_PADDLING_POOL_SEM);
    key_shm = get_key(KEY_PADDLING_POOL_SHM);

    if((PADDLING_POOL_MSQID = msgget(key_msq, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((PADDLING_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, IPC_CREAT|0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((PADDLING_POOL_SHMID = shmget(key_shm, sizeof(int), IPC_CREAT|0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }
}


void setup(){
    __set_shutdown_handler();
    __create_cash_msq();
    __create_pools();
}