#pragma once

#include <stdbool.h>

#include "data_client.h"
#include "logging.h"
#include "low.h"
#include "vars.h"
#include "rand_client.h"
#include "low.h"
#include "leave.h"
#include "keys_id.h"
#include "logs.h"

#include "olimpic/func.h"
#include "paddling/func.h"
#include "leisure/func.h"



void join_paddling_pool(){
    paddling_set_as_cur_pool();

    if(paddling_space_available()){
        paddling_enter_pool();
        LOG_paddling_enter_pool();

        PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
        pool->clients_pids[pool->clients_pids_num] = getpid();
        pool->clients_pids_num++;
        detach_shared_mem(pool);
    }
    else paddling_enter_queue();

    paddling_swim_in_pool();
}


void join_leisure_pool(){
    leisure_set_as_cur_pool();

    if(client_has_child() && leisure_space_available_child() && leisure_below_age_avg_child()){
        leisure_enter_pool_child();
        LOG_leisure_enter_pool_child();

        LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
        pool->clients_pids[pool->clients_pids_num] = getpid();
        pool->clients_pids_num++;
        detach_shared_mem(pool);
    }
    else if(leisure_space_available() && leisure_below_age_avg()){
        leisure_enter_pool();
        LOG_leisure_enter_pool();

        LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
        pool->clients_pids[pool->clients_pids_num] = getpid();
        pool->clients_pids_num++;
        detach_shared_mem(pool);
    }
    else leisure_enter_queue();

    leisure_swim_in_pool();
}


void join_olimpic_pool(){
    olimpic_set_as_cur_pool();

    if(olimpic_space_available()){
        olimpic_enter_pool();
        LOG_olimpic_enter_pool();
        OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
        pool->clients_pids[pool->clients_pids_num] = getpid();
        pool->clients_pids_num++;
        detach_shared_mem(pool);
    }
    else olimpic_enter_queue();
    
    olimpic_swim_in_pool();
}


void choose_pool(){
    if(client_has_baby()){
        join_paddling_pool();
    }

    if(client_is_underage() || client_has_child()){
        join_leisure_pool();
    }


    switch(rand_int(OLIMPIC, LEISURE)){
        case OLIMPIC:
            join_olimpic_pool();

        case LEISURE:
            join_leisure_pool();
    }
}