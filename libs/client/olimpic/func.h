#pragma once

#include "logs.h"
#include "pool.h"
#include "low.h"


void olimpic_enter_pool(){
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