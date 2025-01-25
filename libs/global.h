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


// Process may or may not use it
int KASA_MSQID;


enum GLOBAL_STATE {
    SUCCESS = 0,
    FAILURE = -1,
};


enum GLOBAL_KEYS {
    KASA_KEY,
    ZAMKNIJ_BASENY_KEY,
};


enum MSQ_KASA_STATE {
    MSQ_KASA_WOLNA = 1,
    MSQ_KASA_ZAPLATA,
    MSQ_KASA_RACHUNEK,
};


enum SINGALS {
    SIG_ZAMKNIJ_BASENY = SIGUSR1,
};



enum LOKACJA {
    LOKACJA_KASA,
    LOKACJA_BASEN,
};