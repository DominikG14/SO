#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>


enum SEM__STATES {
    SEM__SUCCESS = 0,
    SEM__FAILURE = -1,
};

enum SEM__OPERATIONS {
    SEM__WAIT = -1,
    SEM__SIGNAL = 1
};


void init_sem(int semid, int nsems){
    int status;

    for (int i = 0; i < nsems; i++) {
        status = semctl(semid, i, SETVAL, 0);
        if(status == SEM__FAILURE) {
            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }
}


int access_sem(key_t key, int nsems, int flags){
    int semid = semget(key, nsems, flags);
    if(semid == SEM__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return semid;
}


void delete_sem(int semid){
    int status = semctl(semid, 0, IPC_RMID);
    if(status == SEM__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


void operate_sem(int semid, int semnum, int sem_op) {
    struct sembuf buffer;
    buffer.sem_num = semnum;
    buffer.sem_op = sem_op;
    buffer.sem_flg = 0;

    int status = semop(semid, &buffer, 1);
    if(status == SEM__FAILURE) {
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


int get_sem_value(int semid, int semnum){
    int value = semctl(semid, semnum, GETVAL);
    if(value == SEM__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return value;
}