#pragma once

#include <sys/stat.h>
#include <sys/types.h>

#include "low.h"

#include "sim_config.h"
#include "pool.h"
#include "vars.h"


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

    semid = access_sem(key, 2, 0600);
    delete_sem(semid);
}


void __create_pool_resources(int pool_num){
    key_t key;
    int semid;

    key = get_key(pool_num);
    access_shared_mem(key, POOL_SHARED_MEM_SIZE[pool_num], IPC_CREAT|0600);
    semid = access_sem(key, 2, IPC_CREAT|0600);
    init_sem(semid, 2);
}


void setup(){
    __load_config();
    __create_tmp_dir();
    signal(SIGUSR1, SIG_IGN); // Signal init
    __create_pool_resources(OLIMPIC);
    __create_pool_resources(LEISURE);
    __create_pool_resources(PADDLING);
}


void clean_up(){
    __delete_pool_resources(OLIMPIC);
    __delete_pool_resources(LEISURE);
    __delete_pool_resources(PADDLING);
}