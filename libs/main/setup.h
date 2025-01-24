#pragma once

#include <sys/stat.h>
#include <sys/types.h>

#include "low.h"

#include "keys_id.h"
#include "sim_config.h"
#include "pool.h"
#include "vars.h"


void __set_main_logger(){
    key_t key = get_key(LOGGING_KEY_ID);
    LOGGER_MSQID = access_msq(key, IPC_CREAT|0600);
}


void __load_config(){
    load_config();

    TIME_PER_SEC = SIM_CONFIG[TIME__PER_SEC];
    TIME_START   = SIM_CONFIG[TIME__START];
    TIME_END     = SIM_CONFIG[TIME__END];

    COMPLEX_OPEN_TIME  = SIM_CONFIG[COMPLEX__OPEN_TIME];
    COMPLEX_CLOSE_TIME = SIM_CONFIG[COMPLEX__CLOSE_TIME];

    CLIENT_SPAWN_PERC = SIM_CONFIG[CLIENT__SPAWN_PERC];

    TIME_CURR = TIME_START;
    COMPLEX_IS_OPEN = false;
}


void __reset_logs(){
    if(remove(LOGGING_FILEPATH) != 0);
}


void __create_tmp_dir(){
    switch(fork()){
        case FORK_FAILURE:
            perror(__func__);
            exit(EXIT_FAILURE);
        
        case FORK_SUCCESS:
            const char *cmd = "/bin/rm";
            char *args[] = {"rm", "-r", "./tmp", NULL};
            execv(cmd, args);
            perror(__func__);
            exit(EXIT_FAILURE);
    }
    sleep(1); // Wait for './tmp' dir to be deleted first
    mkdir("./tmp", 0700);
}


void __delete_pool_resources(int pool_num){
    key_t key;
    int shmid;
    int semid;

    key = get_key(pool_num);
    shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[pool_num], 0600);
    delete_shared_mem(shmid);

    semid = access_sem(key, SEM_POOL_NUM, 0600);
    delete_sem(semid);
}


void __create_pool_resources(){
    key_t key;
    int semid;
    int shmid;


    // Olimpic
    key = get_key(POOL_OLIMPIC_KEY_ID);
    shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[OLIMPIC], IPC_CREAT|0600);
    OlimpicPool* op = get_shared_mem(shmid);
    op->size = 0;
    op->clients_pids_num = 0;
    op->CLOSED = false;
    detach_shared_mem(op);

    semid = access_sem(key, 1, IPC_CREAT|0600);
    init_sem(semid, 1);


    // Leisure
    key = get_key(POOL_LEISURE_KEY_ID);
    shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], IPC_CREAT|0600);
    LeisurePool* lp = get_shared_mem(shmid);
    lp->size = 0;
    lp->age_sum = 0;
    lp->clients_pids_num = 0;
    lp->CLOSED = false;
    detach_shared_mem(lp);

    semid = access_sem(key, 1, IPC_CREAT|0600);
    init_sem(semid, 1);


    // Paddling
    key = get_key(POOL_PADDLING_KEY_ID);
    shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[PADDLING], IPC_CREAT|0600);
    PaddlingPool* pp = get_shared_mem(shmid);
    pp->size = 0;
    pp->clients_pids_num = 0;
    pp->CLOSED = false;
    detach_shared_mem(pp);

    semid = access_sem(key, 1, IPC_CREAT|0600);
    init_sem(semid, 1);
}



void setup(){
    __load_config();
    __create_tmp_dir();
    __reset_logs();
    signal(SIGUSR1, SIG_IGN); // Signal init
    __create_pool_resources(OLIMPIC);
}


void clean_up(){
    __delete_pool_resources(OLIMPIC);
    __delete_pool_resources(LEISURE);
    __delete_pool_resources(PADDLING);
}