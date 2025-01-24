#pragma once

#include <stdbool.h>


#include "logs.h"
#include "pool.h"
#include "client/vars.h"


bool leisure_below_age_avg(){
    return leisure_age_avg(client.age, 1) <= POOL_LEISURE_AGE_AVG;
}

bool leisure_below_age_avg_child(){
    return leisure_age_avg(client.age + child.age, 2) <= POOL_LEISURE_AGE_AVG;
}


bool leisure_space_available(){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    bool open = !(pool->CLOSED);
    detach_shared_mem(pool);
    return cur_size < POOL_SIZE[LEISURE] && open;
}


bool leisure_space_available_child(){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    bool open = !(pool->CLOSED);
    detach_shared_mem(pool);
    return POOL_SIZE[LEISURE] - cur_size >= 2 && open;
}


void leisure_enter_queue(){
    CLIENT_LOCATION = LOCATION__PADDLING_QUEUE;

    if(client_has_child()) LOG_leisure_enter_queue_child();
    else LOG_leisure_enter_queue();

    int status;
    while(true){
        status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

        if(status == SEM_SUCCESS){
            if(client_has_child() && leisure_space_available_child() && leisure_below_age_avg_child()){
                LOG_leisure_leave_queue_child();
                leisure_enter_pool_child();
                LOG_leisure_enter_pool_child();
                break;
            }
            else if(leisure_space_available() && leisure_below_age_avg()){
                LOG_leisure_leave_queue();
                leisure_enter_pool();
                LOG_leisure_enter_pool();
                break;
            } 
            else {
                // operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
                continue;
            }
        }

        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


void leisure_closed_pool(){
    LOG_pool_closed();
    if(client_has_child()) leisure_leave_pool_child();
    else leisure_leave_pool();
    join_leisure_pool();
}


void leisure_set_as_cur_pool(){
    key_t key = get_key(POOL_LEISURE_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], 0600);
}


void leisure_enter_pool(){
    handle_signal(SIG_LIFEGUARD_CLOSE_POOL, leisure_closed_pool);
    CLIENT_LOCATION = LOCATION__LEISURE_POOL;
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    pool->size += 1;
    pool->age_sum += client.age;
    pool->clients_pids[pool->clients_pids_num] = getpid();
    pool->clients_pids_num++;
    detach_shared_mem(pool);
}

void leisure_enter_pool_child(){
    handle_signal(SIG_LIFEGUARD_CLOSE_POOL, leisure_closed_pool);
    CLIENT_LOCATION = LOCATION__LEISURE_POOL;
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    pool->size += 2;
    pool->age_sum += client.age + child.age;
    pool->clients_pids[pool->clients_pids_num] = getpid();
    pool->clients_pids_num++;
    detach_shared_mem(pool);
}


void leisure_leave_pool(){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    pool->size -= 1;
    pool->age_sum -= client.age;
    detach_shared_mem(pool);
}

void leisure_leave_pool_child(){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    pool->size -= 2;
    pool->age_sum -= client.age + child.age;
    detach_shared_mem(pool);
}


void leisure_swim_in_pool(){
    sleep(rand_swim_time());
    if(client_has_child()){
        leisure_leave_pool_child();
        LOG_leisure_leave_pool_child();
        child.SWIM_IN_POOL = false;
        pthread_join(child.tid, NULL);
        operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
    } else {
        leisure_leave_pool();
        LOG_leisure_leave_pool();
    }


    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);

    child.SWIM_IN_POOL = false;
    pthread_join(child.tid, NULL);

    LOG_leave_complex();
    exit(EXIT_SUCCESS);
}