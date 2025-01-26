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


// Local for each process (they may use it or not)
int CASH_MSQID;
int OLIMPIC_POOL_MSQID;
int OLIMPIC_POOL_SEMID;
int OLIMPIC_POOL_SHMID;

int LEISURE_POOL_MSQID;
int LEISURE_POOL_SEMID;
int LEISURE_POOL_SHMID;

int PADDLING_POOL_MSQID;
int PADDLING_POOL_SEMID;
int PADDLING_POOL_SHMID;


char* LOGGING_FILEPATH = "./logs";


enum GLOBAL_STATE {
    SUCCESS = 0,
    FAILURE = -1,
};


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

enum SEM_POOL {
    SEM_POOL_SHM,
    SEM_POOL_NUM
};


enum MSQ_CASH_STATE {
    MSQ_CASH_EMPTY = 1,
    MSQ_CASH_PAY,
    MSQ_CASH_BILL,
};


enum SINGALS {
    SIG_CLOSE_COMPLEX = SIGUSR1,
};


enum POOL_NAME {
    OLIMPIC,
    LEISURE,
    PADDLING,
};