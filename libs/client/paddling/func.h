#pragma once

#include "logs.h"
#include "pool.h"


void paddling_enter_lifeguard_queue(){
    CLIENT_LOCATION = LOCATION__PADDLING_QUEUE;
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__PADDLING_QUEUE,
        REASON__POOL_CLOSED,
        paddling_data,
        STATUS_NONE
    );

    int status;
    while(true){
        status = USoperate_sem(POOL_SEMID, SEM_POOL_LIFEGUARD, SEM_WAIT);

        if(status == SEM_SUCCESS){
            break;
        }

        perror(__func__);
        exit(EXIT_FAILURE);
    }

    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__PADDLING_QUEUE,
        REASON__POOL_CLOSED,
        paddling_data,
        STATUS_LEAVE
    );
    paddling_enter_pool();
    LOG_paddling_enter_pool();
    paddling_swim_in_pool();
}


void paddling_closed_pool(){
    LOG_pool_closed();
    paddling_leave_pool();
    paddling_enter_lifeguard_queue();
}



void paddling_enter_pool(){
    CLIENT_LOCATION = LOCATION__PADDLING_POOL;
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
    pool->size += 2;
    detach_shared_mem(pool);
}


void paddling_enter_queue(){
    LOG_paddling_enter_queue();

    int status;
    while(true){
        status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

        if(status == SEM_SUCCESS){
            LOG_paddling_leave_queue();
            paddling_enter_pool();
            LOG_paddling_enter_pool();
            break;
        }

        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


void paddling_set_as_cur_pool(){
    key_t key = get_key(POOL_PADDLING_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[PADDLING], 0600);
}

bool paddling_space_available(){
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    bool open = !(pool->CLOSED);
    detach_shared_mem(pool);
    return POOL_SIZE[PADDLING] - cur_size >= 2 && open;
}


void paddling_leave_pool(){
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
    pool->size -= 2;
    detach_shared_mem(pool);
}


void paddling_swim_in_pool(){
    sleep(rand_swim_time());
    paddling_leave_pool();
    LOG_paddling_leave_pool();
    
    child.SWIM_IN_POOL = false;
    pthread_join(child.tid, NULL);

    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
    LOG_leave_complex();

    exit(EXIT_SUCCESS);
}