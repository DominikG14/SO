#pragma once

#include <stdbool.h>

#include "vars.h"
#include "low.h"
#include "random.h"
#include "pool.h"
#include "logs.h"
#include "sim_config.h"


void remove_lifeguard(){
    LOG_leave_guarded_pool();
    LOG_leave_complex();

    exit(EXIT_SUCCESS);
}


void setup(){
    srand(getpid());
    handle_signal(SIG_CLOSE_POOL, remove_lifeguard);

    key_t key = get_key(GUARDED_POOL);
    POOL_SHMID = access_shared_mem(key, POOL_NUM, 0600);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);

    load_config();
    LIFEGUARD_CLOSE_POOL_MAX_TIME = SIM_CONFIG[LIFEGUARD__CLOSE_POOL_MAX_TIME] / SIM_CONFIG[TIME__PER_SEC];
    LIFEGUARD_CLOSE_POOL_MIN_TIME = SIM_CONFIG[LIFEGUARD__CLOSE_POOL_MIN_TIME] / SIM_CONFIG[TIME__PER_SEC];
    LIFEGUARD_CLOSE_POOL_PERC     = SIM_CONFIG[LIFEGUARD__CLOSE_POOL_PERC];

    POOL_SIZE[OLIMPIC] = SIM_CONFIG[POOL__OLIMPIC_MAX_SIZE];
    POOL_SIZE[LEISURE] = SIM_CONFIG[POOL__LEISURE_MAX_SIZE];
    POOL_SIZE[PADDLING] = SIM_CONFIG[POOL__PADDLING_MAX_SIZE];
}


void close_pool(){
    switch(GUARDED_POOL){
        case OLIMPIC:
            OlimpicPool* op =(OlimpicPool*) get_shared_mem(POOL_SHMID);
            op->CLOSED = true;
            for(int i = 0; i < op->clients_pids_num; i++){
                send_signal(op->clients_pids[i], SIG_LIFEGUARD_CLOSE_POOL);
            }
            detach_shared_mem(op);
            break;
        

        case LEISURE:
            LeisurePool* lp =(LeisurePool*) get_shared_mem(POOL_SHMID);
            lp->CLOSED = true;
            for(int i = 0; i < lp->clients_pids_num; i++){
                send_signal(lp->clients_pids[i], SIG_LIFEGUARD_CLOSE_POOL);
            }
            detach_shared_mem(lp);
            break;


        case PADDLING:
            PaddlingPool* pp =(PaddlingPool*) get_shared_mem(POOL_SHMID);
            pp->CLOSED = true;
            for(int i = 0; i < pp->clients_pids_num; i++){
                send_signal(pp->clients_pids[i], SIG_LIFEGUARD_CLOSE_POOL);
            }
            detach_shared_mem(pp);
            break;
    }
}

void open_pool(){
    switch(GUARDED_POOL){
        case OLIMPIC:
            while(USget_sem_waiting_count(POOL_SEMID, SEM_POOL_LIFEGUARD) > 0){
                operate_sem(POOL_SEMID, SEM_POOL_LIFEGUARD, SEM_SIGNAL);
            }

            OlimpicPool* op =(OlimpicPool*) get_shared_mem(POOL_SHMID);
            op->CLOSED = false;
            detach_shared_mem(op);
            break;
        

        case LEISURE:
            while(USget_sem_waiting_count(POOL_SEMID, SEM_POOL_LIFEGUARD) > 0){
                operate_sem(POOL_SEMID, SEM_POOL_LIFEGUARD, SEM_SIGNAL);
            }

            LeisurePool* lp =(LeisurePool*) get_shared_mem(POOL_SHMID);
            lp->CLOSED = false;
            detach_shared_mem(lp);
            break;


        case PADDLING:
            while(USget_sem_waiting_count(POOL_SEMID, SEM_POOL_LIFEGUARD) > 0){
                operate_sem(POOL_SEMID, SEM_POOL_LIFEGUARD, SEM_SIGNAL);
            }
            
            PaddlingPool* pp =(PaddlingPool*) get_shared_mem(POOL_SHMID);
            pp->CLOSED = false;
            detach_shared_mem(pp);
            break;
    }
}