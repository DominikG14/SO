#pragma once

#include <stdbool.h>


#include "logs.h"
#include "pool.h"
#include "client/vars.h"


void leisure_set_as_cur_pool(){
    key_t key = get_key(POOL_LEISURE_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], 0600);
}

bool leisure_space_available(){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    detach_shared_mem(pool);
    return cur_size < POOL_SIZE[LEISURE];
}

bool leisure_space_available_child(){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    detach_shared_mem(pool);
    return POOL_SIZE[LEISURE] - cur_size >= 2;
}

bool leisure_below_age_avg(){
    return leisure_age_avg(client.age, 1) <= POOL_LEISURE_AGE_AVG;
}

bool leisure_below_age_avg_child(){
    return leisure_age_avg(client.age + child.age, 2) <= POOL_LEISURE_AGE_AVG;
}

void leisure_enter_pool(){
    CLIENT_LOCATION = LOCATION__LEISURE_POOL;
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    pool->size += 1;
    pool->age_sum += client.age;
    pool->clients_pids[pool->clients_pids_num] = getpid();
    pool->clients_pids_num++;
    detach_shared_mem(pool);
}

void leisure_enter_pool_child(){
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