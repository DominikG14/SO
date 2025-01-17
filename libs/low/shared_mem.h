#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>


enum SHARED_MEM__STATES {
    SHARED_MEM__SUCCESS = 0,
    SHARED_MEM__FAILURE = -1,
};


int access_shared_mem(key_t key, int size, int flags){
    int shmid = shmget(key, size, flags);
    if(shmid == SHARED_MEM__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return shmid;
}


void* get_shared_mem(int shmid){
    void* shmaddr =(void*) shmat(shmid, NULL, 0);
    if(shmaddr ==(void*) SHARED_MEM__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return shmaddr;
}


void detach_shared_mem(void* shmaddr){
    int status = shmdt(shmaddr);
    if(status == SHARED_MEM__FAILURE) {
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


void delete_shared_mem(int shmid) {
    int status = shmctl(shmid, IPC_RMID, NULL);
    if(status == SHARED_MEM__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}