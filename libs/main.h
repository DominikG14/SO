#pragma once

#include "colors.h"
#include "global.h"
#include "low.h"


// -------------------- Processes data --------------------
int PID_CASHIER;

int PID_CLIENTS[1000];
int CLIENTS_NUM = 0;


// -------------------- Pool Complex functionality --------------------
void open_pools(){
    MSQ_BUFFER.mtype=MSQ_POOL_SPACE;


    // Olimpic pool
            // Each client comes alone
    for(int i = 0; i < POOL_OLIMPIC_MAX_SIZE; i++){
        if(msgsnd(OLIMPIC_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("main - msgsnd - open_pool");
            exit(EXIT_FAILURE);
        }
    }
    semctl(OLIMPIC_POOL_SEMID, SEM_POOL_SHM, SETVAL, SEM_SIGNAL);


    // Leisure pool
            // Clients come with childs or alone
    for(int i = 0; i < POOL_LEISURE_MAX_SIZE; i++){
        if(msgsnd(LEISURE_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("main - msgsnd - open_pool");
            exit(EXIT_FAILURE);
        }
    }
    semctl(LEISURE_POOL_SEMID, SEM_POOL_SHM, SETVAL, SEM_SIGNAL);


    // Paddling pool
            // Each client comes with a child
    for(int i = 0; i < POOL_PADDLING_MAX_SIZE / 2; i++){
        if(msgsnd(PADDLING_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("main - msgsnd - open_pool");
            exit(EXIT_FAILURE);
        }
    }
    semctl(PADDLING_POOL_SEMID, SEM_POOL_SHM, SETVAL, SEM_SIGNAL);

}


// -------------------- IPCS --------------------
void clean_up(){
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
    clean_up();
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
    int* pool;

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
    pool =(int*) shmat(OLIMPIC_POOL_SHMID, NULL, 0);
    pool = 0;
    shmdt(pool);


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
    pool =(int*) shmat(LEISURE_POOL_SHMID, NULL, 0);
    pool[LEISURE_POOL_SIZE] = 0;
    pool[LEISURE_POOL_AGE_SUM] = 0;
    shmdt(pool);


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
    pool =(int*) shmat(PADDLING_POOL_SHMID, NULL, 0);
    pool = 0;
    shmdt(pool);
}


void setup(){
    __set_shutdown_handler();
    __create_cash_msq();
    __create_pools();
}