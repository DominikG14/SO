#pragma once

#include "logs.h"
#include "pool.h"


void paddling_set_as_cur_pool(){
    key_t key = get_key(POOL_PADDLING_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[PADDLING], 0600);
}

bool paddling_space_available(){
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    detach_shared_mem(pool);
    return POOL_SIZE[PADDLING] - cur_size >= 2;
}

void paddling_enter_pool(){
    CLIENT_LOCATION = LOCATION__PADDLING_POOL;
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
    pool->size += 2;
    detach_shared_mem(pool);
}

void paddling_leave_pool(){
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);
    pool->size -= 2;
    detach_shared_mem(pool);
}