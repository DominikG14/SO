#pragma once

#include "logs.h"
#include "pool.h"
#include "low.h"


void olimpic_enter_pool(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    pool->size += 1;
    detach_shared_mem(pool);
}

bool olimpic_space_available(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    detach_shared_mem(pool);
    return cur_size < POOL_SIZE[CURRENT_POOL];
}

void olimpic_set_as_cur_pool(){
    CURRENT_POOL = OLIMPIC;

    // Generate access to pool resources
    key_t key  = get_key(POOL_OLIMPIC_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[CURRENT_POOL], 0600);
}


void olimpic_leave_pool(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    pool->size -= 1;
    detach_shared_mem(pool);
}