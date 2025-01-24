#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>


#define OPERATE_SEM(semid, semnum, sem_op) operate_sem(semid, semnum, sem_op, true);


enum SEM__STATES {
    SEM_SUCCESS = 0,
    SEM_FAILURE = -1,
};

enum SEM__OPERATIONS {
    SEM_WAIT = -1,
    SEM_SIGNAL = 1
};


void init_sem(int semid, int nsems){
    int status;

    for (int i = 0; i < nsems; i++) {
        status = semctl(semid, i, SETVAL, 0);
        if(status == SEM_FAILURE) {
            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }
}


int access_sem(key_t key, int nsems, int flags){
    int semid = semget(key, nsems, flags);
    if(semid == SEM_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return semid;
}


void delete_sem(int semid){
    int status = semctl(semid, 0, IPC_RMID);
    if(status == SEM_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


int operate_sem(int semid, int semnum, int sem_op){
    struct sembuf buffer;
    buffer.sem_num = semnum;
    buffer.sem_op = sem_op;
    buffer.sem_flg = 0;

    int status = semop(semid, &buffer, 1);
    if(status == SEM_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return status;
}


int USoperate_sem(int semid, int semnum, int sem_op){
    struct sembuf buffer;
    buffer.sem_num = semnum;
    buffer.sem_op = sem_op;
    buffer.sem_flg = 0;

    int status = semop(semid, &buffer, 1);
    return status;
}



int USget_sem_value(int semid, int semnum){
    int status = semctl(semid, semnum, GETVAL);
    return status;
}


int USget_sem_waiting_count(int semid, int semnum) {
    int waiting_count = semctl(semid, semnum, GETNCNT);
    return waiting_count;
}