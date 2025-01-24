#pragma once
#define POOL_MAX_CLIENTS 1500

#include <stdbool.h>


enum POOL__TYPES {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOL_NUM,
};


struct POOL_OLIMPIC_SHARED_MEM {
    int size;
    int clients_pids[POOL_MAX_CLIENTS];
    int clients_pids_num;
    bool CLOSED;
} typedef OlimpicPool;


struct POOL_LEISURE_SHARED_MEM {
    int size;
    int age_sum;
    int clients_pids[POOL_MAX_CLIENTS];
    int clients_pids_num;
    bool CLOSED;
} typedef LeisurePool;


struct POOL_PADDLING_SHARED_MEM {
    int size;
    int clients_pids[POOL_MAX_CLIENTS];
    int clients_pids_num;
    bool CLOSED;
} typedef PaddlingPool;


int POOL_SHARED_MEM_SIZE[] = {
    sizeof(struct POOL_OLIMPIC_SHARED_MEM),
    sizeof(struct POOL_LEISURE_SHARED_MEM),
    sizeof(struct POOL_PADDLING_SHARED_MEM),
};