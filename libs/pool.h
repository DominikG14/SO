#pragma once


enum POOL__TYPES {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOL_NUM,
};

const int POOL_OLIMPIC_MAX_SIZE  = 10;
const int POOL_LEISURE_MAX_SIZE  = 10;
const int POOL_PADDLING_MAX_SIZE = 10;


int POOL_SIZE[] = {
    POOL_OLIMPIC_MAX_SIZE,
    POOL_LEISURE_MAX_SIZE,
    POOL_PADDLING_MAX_SIZE,
};


struct POOL_OLIMPIC_SHARED_MEM {
    int size;
} typedef OlimpicPool;


struct POOL_LEISURE_SHARED_MEM {
    int size;
    int age_sum;
} typedef LeisurePool;


struct POOL_PADDLING_SHARED_MEM {
    int size;
} typedef PaddlingPool;


// Global pools objects
OlimpicPool  olimpic_pool;
LeisurePool  leisure_pool;
PaddlingPool paddling_pool;