#pragma once

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>

#define MAX_CLIENTS_PER_DAY 1440 // num of minutes in one day


// -------------------- Keys --------------------
enum GLOBAL_KEYS {
    KEY_CASH_MSQ,

    // Olimpic pool
    KEY_OLIMPIC_POOL_MSQ,
    KEY_OLIMPIC_POOL_SEM,
    KEY_OLIMPIC_POOL_SHM,

    // Leisure pool
    KEY_LEISURE_POOL_MSQ,
    KEY_LEISURE_POOL_SEM,
    KEY_LEISURE_POOL_SHM,

    // Paddling pool
    KEY_PADDLING_POOL_MSQ,
    KEY_PADDLING_POOL_SEM,
    KEY_PADDLING_POOL_SHM,
};


// -------------------- Local --------------------
// for each process (they may use it or not)
int CASH_MSQID;
int POOL_MSQID;
int POOL_SEMID;
int POOL_SHMID;

int POOL_SIZE; // Current pool size for logging
int POOL_AGE_SUM; // For leisure pool age avg 


struct sembuf SEM_OPERATE;


// -------------------- Pool Data --------------------
struct PoolData {
    int size;
    int age_sum; // Only for Leisure pool
    int pid_clients[MAX_CLIENTS_PER_DAY];
} typedef PoolData;


enum POOL_NAME {
    OLIMPIC,
    LEISURE,
    PADDLING,
};


// -------------------- Message queue operations --------------------
enum MSQ_CASH_STATE {
    MSQ_CASH_EMPTY = 1,
    MSQ_CASH_PAY,
    MSQ_CASH_BILL,
};


enum MSQ_POOL_STATE {
    MSQ_POOL_SPACE = 1,
    MSQ_LIFEGUARD,
};


// -------------------- Sem operations --------------------
enum SEM__OPERATIONS {
    SEM_WAIT = -1,
    SEM_SIGNAL = 1,
};


enum SEM_POOL {
    SEM_POOL_SHM,
    SEM_POOL_LIFEGUARD,
    SEM_POOL_NUM
};


// -------------------- Signals --------------------
enum SINGALS {
    SIG_CLOSE_COMPLEX = SIGUSR1,
    SIG_LIFEGUARD = SIGUSR2,
};