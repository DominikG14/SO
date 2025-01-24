#pragma once

#include "logs.h"
#include "pool.h"
#include "low.h"


void olimpic_closed_pool(){
    LOG_pool_closed();
    olimpic_leave_pool();
    olimpic_enter_queue();
    join_olimpic_pool();
}


void olimpic_enter_queue(){
    CLIENT_LOCATION = LOCATION__OLIMPIC_QUEUE;
    LOG_olimpic_enter_queue();

    int status;
    while(true){
        status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

        if(status == SEM_SUCCESS){
            LOG_olimpic_leave_queue();
            olimpic_enter_pool();
            LOG_olimpic_enter_pool();
            break;
        }

        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


void olimpic_enter_pool(){
    handle_signal(SIG_LIFEGUARD_CLOSE_POOL, olimpic_closed_pool);
    CLIENT_LOCATION = LOCATION__OLIMPIC_POOL;
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    pool->size += 1;
    pool->clients_pids[pool->clients_pids_num] = getpid();
    pool->clients_pids_num++;
    detach_shared_mem(pool);
}


bool olimpic_space_available(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    detach_shared_mem(pool);
    return cur_size < POOL_SIZE[OLIMPIC];
}


void olimpic_set_as_cur_pool(){
    key_t key  = get_key(POOL_OLIMPIC_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[OLIMPIC], 0600);
}


void olimpic_leave_pool(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    pool->size -= 1;
    detach_shared_mem(pool);
}


void olimpic_swim_in_pool(){
    sleep(rand_swim_time());
    olimpic_leave_pool();
    LOG_olimpic_leave_pool();
    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);

    LOG_leave_complex();

    exit(EXIT_SUCCESS);
}