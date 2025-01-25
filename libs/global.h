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


// Process may or may not use it
int CASH_MSQID;


enum GLOBAL_STATE {
    SUCCESS = 0,
    FAILURE = -1,
};


enum GLOBAL_KEYS {
    KEY_CASH_MSQ,
    
    KEY_OLIMPIC_POOL_MSQ,
    KEY_OLIMPIC_POOL_SEM,
    KEY_OLIMPIC_POOL_SHM,
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


enum LOCATION {
    LOCATION_CASH_QUEUE,
    LOCATION_OLIMPIC_POOL,
    LOCATION_LEISURE_POOL,
    LOCATION_PADDLING_POOL,
};